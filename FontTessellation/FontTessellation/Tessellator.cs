/**************************************************************************
 * (c) 2011 Laurent Dupuis
 * ........................................................................
 * < This program is free software: you can redistribute it and/or modify
 * < it under the terms of the GNU General Public License as published by
 * < the Free Software Foundation, either version 3 of the License, or
 * < (at your option) any later version.
 * < 
 * < This program is distributed in the hope that it will be useful,
 * < but WITHOUT ANY WARRANTY; without even the implied warranty of
 * < MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * < GNU General Public License for more details.
 * < 
 * < You should have received a copy of the GNU General Public License
 * < along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ........................................................................
 */
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using Poly2Tri;
using System.IO;
namespace FontTessellation
{
    class Tessellator : Control
    {
        static readonly Pen[] PathPens = new[]
                                             {
                                                 Pens.Red, 
                                                 Pens.Green, 
                                                 Pens.Blue, 
                                                 Pens.Violet, 
                                                 Pens.Black,
                                                 Pens.DarkRed,
                                                 Pens.DarkGreen,
                                                 Pens.DarkBlue
                                             };
        const float GlyphFontSize = 128f;
        const float SzP = 1.5f;
        const float TgMulti = 2.5f;
        string _font;
        char   _glyph;
        readonly Font   _txtFont;

        public Tessellator()
        {
            BackColor = Color.White;
            _txtFont = new Font(FontFamily.GenericSansSerif,8f);
        }

        public void DoTesselation(string font,char glyph)
        {
            _font = font;
            _glyph = glyph;
            Invalidate();
        }

        /// <summary>
        /// Check if a point is in a polygon
        /// </summary>
        /// <param name="p">Point to check</param>
        /// <param name="poly">Container polygon candidate</param>
        /// <returns>true in, false out</returns>
        static bool PointInPolygon(TriangulationPoint p, IList<TriangulationPoint> poly)
        {
            PolygonPoint p1, p2;
            var inside = false;
            var oldPoint = new PolygonPoint(poly[poly.Count - 1].X, poly[poly.Count - 1].Y);

            for (var i = 0; i < poly.Count; i++)
            {
                var newPoint = new PolygonPoint(poly[i].X, poly[i].Y);
                if (newPoint.X > oldPoint.X) { p1 = oldPoint; p2 = newPoint; }
                else                         { p1 = newPoint; p2 = oldPoint; }
                if ((newPoint.X < p.X) == (p.X <= oldPoint.X) && ((long)p.Y - (long)p1.Y) * (long)(p2.X - p1.X)
                     < ((long)p2.Y - (long)p1.Y) * (long)(p.X - p1.X))
                {
                    inside = !inside;
                }
                oldPoint = newPoint;
            }
            return inside;
        }

        /// <summary>
        /// Check if a polygon is inside another. 
        /// </summary>
        /// <param name="polygonToTest"></param>
        /// <param name="containingPolygon"></param>
        /// <returns>true if at least 60% of the points are inside</returns>
        static bool CheckIfInside(
            IList<TriangulationPoint> polygonToTest, 
            IList<TriangulationPoint> containingPolygon)
        {
            var t = 0;
            for (var i = 0; i < polygonToTest.Count; ++i)
            {
                if (PointInPolygon(polygonToTest[i], containingPolygon)) t++;
            }

            return ((float)t) >= (polygonToTest.Count * .6f) ? true : false;
        }

        class PolygonHierachy
        {
            public Polygon Current;
            public List<PolygonHierachy> Childs;
            public PolygonHierachy Next;

            public PolygonHierachy(Polygon current)
            {
                Current = current;
                Childs = new List<PolygonHierachy>();
                Next = null;
            }
        }

        static void ProcessLevel(Polygon poly,ref PolygonHierachy localRoot)
        {
            if(localRoot==null)
            {
                localRoot = new PolygonHierachy(poly);
                return;
            }

            // Check if source is the new root
            if (CheckIfInside(localRoot.Current.Points, poly.Points))
            {
                var nroot = new PolygonHierachy(poly);
                var tmp = localRoot;
                while (tmp!=null)
                {
                    var cur = tmp;
                    tmp = tmp.Next;
                    cur.Next = null;
                    nroot.Childs.Add(cur);
                }
                
                localRoot = nroot;
                return;
            }

            // Check if source is not in the local root
            if (!CheckIfInside(poly.Points, localRoot.Current.Points))
            {
                ProcessLevel(poly, ref localRoot.Next);
                return;
            }

            // Now process the childs
            for (var i = 0; i < localRoot.Childs.Count;++i )
            {
                if (!CheckIfInside(poly.Points, localRoot.Childs[i].Current.Points)) continue;

                // Process to the child level
                var childRoot = localRoot.Childs[i];
                ProcessLevel(poly, ref childRoot);
                localRoot.Childs[i] = childRoot;
                return;
            }

            // Else -> new child
            var newChildList = new List<PolygonHierachy>();
            var newPoly = new PolygonHierachy(poly);
            newChildList.Add(newPoly);
            for (var i = 0; i < localRoot.Childs.Count; ++i)
            {
                if(CheckIfInside(localRoot.Childs[i].Current.Points,poly.Points))
                {
                    newPoly.Childs.Add(localRoot.Childs[i]);
                }
                else
                {
                    newChildList.Add(localRoot.Childs[i]);
                }
            }

            localRoot.Childs = newChildList; //.Childs.Add(new PolygonHierachy(poly));
        }

        static void ProcessSetLevel(PolygonSet set,PolygonHierachy current)
        {
            while (current!=null)
            {
                var poly = current.Current;
                foreach (var child in current.Childs)
                {
                    poly.AddHole(child.Current);
                    foreach (var grandchild in child.Childs) ProcessSetLevel(set, grandchild);
                }
                set.Add(poly);
                current = current.Next;
            }
        }

        static PolygonSet CreateSetFromList(IList<Polygon> source)
        {
            // First we need to reorganize the polygons
            var root = new PolygonHierachy(source[0]);

            for(var i=1;i<source.Count;++i)
            {
                ProcessLevel(source[i], ref root);
            }

            // Generate the set from the hierachy
            var set = new PolygonSet();
            ProcessSetLevel(set, root);

            return set;
        }

        public struct Face
        {
            public int x;
            public int y;
            public int z;

            public Face(int _x=0, int _y=0, int _z=0)
            {
                x = _x;
                y = _y;
                z = _z;
            }
        };
        private void GetTrianglesBetweenTwoZPlane(List<List<PolygonPoint>> points_list,double z1,double z2, List<double> triangles_xyz_array)
        {
            List<double> points_array = new List<double>();
            List<int> triangles_array=new List<int>();
            for(int i=0; i<points_list.Count; i++)
            {
                int idStart = points_array.Count / 3;
                for(int j=0; j<points_list[i].Count; j++)
                {
                    double x=(points_list[i][j].X);
                    double y = (points_list[i][j].Y);
                    points_array.Add(x);
                    points_array.Add(y);
                    points_array.Add(z1);

                    points_array.Add(x);
                    points_array.Add(y);
                    points_array.Add(z2);
                }
                int idEnd = points_array.Count / 3;
                //square 0-1-2-3, 2-3-4-5
                for(int id=idStart; id<idEnd-2; id+=2)
                {
                    //Add two triangles
                    //int [] arr=new int[6]{id,id+1,id+3,id,id+3,id+2};
                    int[] arr = new int[6] { id, id + 3, id + 1, id, id + 2, id + 3};
                    AddTriangles(ref triangles_xyz_array, points_array, arr);
                }
                //Connect idStart and idEnd, add two triangles
                //int[] arr_last = new int[6] { idEnd - 2, idEnd - 1, idStart + 1, idEnd - 2, idStart + 1, idStart };
                int[] arr_last = new int[6] { idEnd - 2, idStart + 1, idEnd - 1, idEnd - 2, idStart, idStart + 1 };
                AddTriangles(ref triangles_xyz_array, points_array, arr_last);
            }
        }

        private void AddTriangles(ref List<double> triangles_xyz_array, List<double> points_array, int[] arr)
        {
            for (int j = 0; j < arr.Length; j++)
            {
                triangles_xyz_array.Add(points_array[3 * arr[j]]);
                triangles_xyz_array.Add(points_array[3 * arr[j] + 1]);
                triangles_xyz_array.Add(points_array[3 * arr[j] + 2]);
            }
        }
        /// <summary>
        /// Construct the "raw" (no hole, unsorted) polygon list from the GDI+ paths
        /// </summary>
        /// <param name="fx">Graphics for debug output</param>
        /// <param name="fontFamily">Font family</param>
        /// <param name="style">Font style</param>
        /// <param name="glyph">String containing the glyph to write</param>
        /// <returns>List of polygon</returns>
        static List<Polygon> GeneratePolygonsFromGlyph(Graphics fx, 
            FontFamily fontFamily, FontStyle style,
            string glyph,ref List<List<PolygonPoint>> points_list)
        {
            PointF[] pts = null;
            byte[] ptsType = null;

            using (var path = new GraphicsPath()) 
            {
                path.AddString(glyph, fontFamily, (int)style, GlyphFontSize, 
                    new PointF(0f, 0f), StringFormat.GenericDefault);

                path.Flatten();

                if(path.PointCount==0) return new List<Polygon>();

                pts     = path.PathPoints;
                ptsType = path.PathTypes;
            }

            var polygons = new List<Polygon>();
            List<PolygonPoint> points = null;
            Pen cPen=Pens.Yellow;
            var ip = 0;
            var start = -1;

            int count = 0;
            for (var i = 0; i < pts.Length; i++)
            {
                var pointType = ptsType[i] & 0x07;
                if (pointType == 0)
                {
                    points = new List<PolygonPoint> { new PolygonPoint(pts[i].X, pts[i].Y) };
                    start = i;
                    cPen = PathPens[ip % PathPens.Length]; ++ip;
                    fx.FillRectangle(Brushes.DarkOrange, pts[i].X - SzP, pts[i].Y - SzP, SzP * 2, SzP * 2);
                    continue;
                }
                if (pointType != 1) throw new Exception("Unsupported point type");

                fx.DrawLine(cPen, pts[i - 1], pts[i]);

                if ((ptsType[i] & 0x80) != 0)
                {
                    //- Last point in the polygon
                    if(pts[i]!=pts[start])
                    {
                        points.Add(new PolygonPoint(pts[i].X, pts[i].Y));
                    }
                    polygons.Add( new Polygon( points ));
                    points_list.Add(points);
                    count++;
                    fx.FillRectangle(Brushes.DarkBlue, pts[i].X - SzP, pts[i].Y - SzP, SzP * 2, SzP * 2);
                    fx.DrawLine(cPen, pts[i], pts[start]);

                    points = null;
                }
                else
                {
                    points.Add(new PolygonPoint(pts[i].X, pts[i].Y) );
                }
            }
            return polygons;
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            var fx = e.Graphics;
            if(string.IsNullOrEmpty(_font)) return;

            var ffamilly = new FontFamily(_font);
            FontStyle? fstyle = null;
            foreach (var style in new[] { FontStyle.Regular, FontStyle.Bold, FontStyle.Italic, FontStyle.Underline, FontStyle.Strikeout })
            {
                if (!ffamilly.IsStyleAvailable(style)) continue;
                fstyle = style;
                break;
            }
            if(!fstyle.HasValue) return;

            var strChar = new string(_glyph, 1);
            var txtEM = fx.MeasureString("M", _txtFont).Height;
            SizeF szGlyph;

            fx.DrawString("Reference from GDI+",_txtFont,Brushes.Black,0f,0f);
            using (var tmpFnt = new Font(ffamilly, GlyphFontSize, fstyle.Value))
            {
                szGlyph = fx.MeasureString(strChar, tmpFnt);
                fx.DrawString(strChar, tmpFnt, Brushes.Black, 0f, txtEM);
                fx.DrawRectangle(Pens.Blue, 0f, txtEM, szGlyph.Width, szGlyph.Height);
            }

            fx.TranslateTransform(0f, szGlyph.Height + txtEM*2f);
            fx.DrawRectangle(Pens.Blue, 0f, 0f, szGlyph.Width, szGlyph.Height);

            {
                List<List<PolygonPoint>> points_list = new List<List<PolygonPoint>>();
                var polygonList_bottom = GeneratePolygonsFromGlyph(fx, ffamilly, fstyle.Value, "管孟ABC12334455", ref points_list);

                double[] min;
                double[] max;
                GetBoundingBox(ref points_list,out min, out max);

                var polygonList_base=PolygonsInBox(ref polygonList_bottom,min, max);

                var polygonSet_bottom = CreateSetFromList(polygonList_bottom);
                var polygonSet_base = CreateSetFromList(polygonList_base);

                P2T.Triangulate(polygonSet_bottom);
                P2T.Triangulate(polygonSet_base);
                CarveMesh(polygonSet_bottom, polygonSet_base, points_list, "d:\\temp\\triangles_all2.stl");
            }
            try
            {
                List<List<PolygonPoint>> points_list=new List<List<PolygonPoint>>();
                //var polygonList = GeneratePolygonsFromGlyph(fx, ffamilly, fstyle.Value, strChar);
                var polygonList_bottom = GeneratePolygonsFromGlyph(fx, ffamilly, fstyle.Value, "管孟辉",ref points_list);

                double[] min;
                double[] max;
                GetBoundingBox(ref points_list, out min, out max);
                var polygonList_base = PolygonsInBox(ref polygonList_bottom,min,max);
                
                fx.ResetTransform();
                fx.DrawString(
                    string.Format("{0} Paths from GDI:", polygonList_bottom.Count), 
                    _txtFont, Brushes.Black, 0f, szGlyph.Height + txtEM);

                if(polygonList_bottom.Count==0)
                {
                    fx.DrawString(
                        "Empty Path - Nothing to Tessellate ....",
                        _txtFont, Brushes.DarkGreen, szGlyph.Width + 2 * txtEM, 0f);
                    return;
                }

                var polygonSet_bottom = CreateSetFromList(polygonList_bottom);
                var polygonSet_base = CreateSetFromList(polygonList_base);
                
                P2T.Triangulate(polygonSet_bottom);
                P2T.Triangulate(polygonSet_base);

                fx.TranslateTransform(szGlyph.Width + 2*txtEM, txtEM);

                var sb = new SolidBrush(Color.FromArgb(128, Color.Gray));
                var ip = 0;
                var totalTriangles=0;

                foreach (var pol in polygonSet_bottom.Polygons)
                {
                    var cPen = PathPens[ip%PathPens.Length];
                    ++ip;
                    foreach (var tri in pol.Triangles)
                    {
                        var ptfs = new PointF[]
                                       {
                                           new PointF(tri.Points[0].Xf*TgMulti, tri.Points[0].Yf*TgMulti)
                                           , new PointF(tri.Points[1].Xf*TgMulti, tri.Points[1].Yf*TgMulti)
                                           , new PointF(tri.Points[2].Xf*TgMulti, tri.Points[2].Yf*TgMulti)
                                       };
                        fx.FillPolygon(sb, ptfs);
                        fx.DrawPolygon(cPen, ptfs);
                        ++totalTriangles;
                    }
                }

                fx.ResetTransform();
                fx.DrawString(
                    string.Format("Polygons: {0} / Total Triangles: {1}",
                                  ((System.Collections.ICollection)polygonSet_bottom.Polygons).Count, 
                                  totalTriangles),
                    _txtFont, Brushes.Black, szGlyph.Width + 2 * txtEM, 0f);
            }
            catch (Exception ex)
            {
                fx.ResetTransform();
                fx.DrawString("EXCEPTION!!", _txtFont, Brushes.Red, szGlyph.Width + 2 * txtEM, 0f);
                fx.DrawString(ex.ToString(), _txtFont, Brushes.Red, szGlyph.Width + 2 * txtEM, txtEM);
            }

        }

        private void GetBoundingBox(ref List<List<PolygonPoint>> points_list, out double[] min, out double[] max)
        {
            min = new double[] { 100, 100 };
            max = new double[] { 0, 0 };
            double[] cur = new double[2];
            for (int i = 0; i < points_list.Count; i++)
            {
                for (int j = 0; j < points_list[i].Count; j++)
                {
                    cur[0] = points_list[i][j].X;
                    cur[1] = points_list[i][j].Y;

                    for (int k = 0; k < 2; k++)
                    {
                        if (cur[k] < min[k]) min[k] = cur[k];
                        if (cur[k] > max[k]) max[k] = cur[k];
                    }
                }
            }
        }
        private List<Polygon> PolygonsInBox(ref List<Polygon> polygonList_bottom, double[] min, double[] max)
        {
            var polygonList_base = new List<Polygon>();
            for (int i = 0; i < polygonList_bottom.Count; i++)
            {
                polygonList_base.Add(polygonList_bottom[i]);
            }
            {
                double x1 = Math.Max(0, min[0] - 10), y1 = Math.Max(0, min[1] - 10), x2 = max[0] + 10, y2 = max[1] + 10;
                //double x1 = 0, y1 = 0, x2 = 650, y2 = 350;
                var points = new List<PolygonPoint>();
                points.Add(new PolygonPoint(x1, y1));
                points.Add(new PolygonPoint(x2, y1));
                points.Add(new PolygonPoint(x2, y2));
                points.Add(new PolygonPoint(x1, y2));

                polygonList_base.Add(new Polygon(points));
            }
            return polygonList_base;
        }
        private void CarveMesh(PolygonSet polygonSet_bottom, PolygonSet polygonSet_base, List<List<PolygonPoint>> points_list,string filename)
        {
            List<double> triangels = new List<double>();
            double z_bottom = 10;
            double z_base = 0;
            FromPolygonSetToTriangles(polygonSet_bottom, z_bottom,  triangels);
            FromPolygonSetToTriangles(polygonSet_base, z_base,  triangels);
            GetTrianglesBetweenTwoZPlane(points_list, z_base, z_bottom, triangels);
            WriteStl(triangels, filename);
        }
        private void FromPolygonSetToTriangles(PolygonSet polygonSet2,double z, List<double> triangels)
        {
            foreach (var pol in polygonSet2.Polygons)
            {
                foreach (var tri in pol.Triangles)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        triangels.Add(tri.Points[i].X);
                        triangels.Add(tri.Points[i].Y);
                        triangels.Add(z);
                    }
                }
            }
        }
        private void WriteStl(List<double> triangles_xyz_array, string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine("solid STL generated by MeshLab");

            for (int i = 0; i < triangles_xyz_array.Count; i+=9)
            {
                sw.WriteLine("  facet normal 0 0 -1");
                sw.WriteLine("    outer loop");
                
                int []order=new int[]{1,0,2};//reverse triangle orientation
                for(int j=0; j<3; j++)
                {
                    int orderId = order[j];
                    sw.WriteLine("      vertex  {0} {1} {2}", triangles_xyz_array[i + 3 * orderId], triangles_xyz_array[i + 3 * orderId + 1], triangles_xyz_array[i + 3 * orderId + 2]);
                }
                sw.WriteLine("    endloop");
                sw.WriteLine("  endfacet");
            }
            sw.Close();
        }

        private void WritePly(ref List<PointF> points_array, ref List<int> triangles_array,string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine("ply");
            sw.WriteLine("format ascii 1.0");
            sw.WriteLine("element vertex {0}", points_array.Count);
            sw.WriteLine("property float x");
            sw.WriteLine("property float y");
            sw.WriteLine("property float z");
            sw.WriteLine("element face {0}", triangles_array.Count / 3);
            sw.WriteLine("property list uchar int vertex_indices");
            sw.WriteLine("end_header");
            for (int i = 0; i < points_array.Count; i++)
            {
                sw.WriteLine("{0} {1} {2}", points_array[i].X, 500 - points_array[i].Y, 0);
            }
            for (int i = 0; i < triangles_array.Count / 3; i++)
            {
                sw.WriteLine("3 {0} {1} {2}", triangles_array[3 * i + 1], triangles_array[3 * i], triangles_array[3 * i + 2]);
            }
            sw.Close();
        }

        private void WritePly(ref List<double> points_array, ref List<int> triangles_array, string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine("ply");
            sw.WriteLine("format ascii 1.0");
            sw.WriteLine("element vertex {0}", points_array.Count/3);
            sw.WriteLine("property float x");
            sw.WriteLine("property float y");
            sw.WriteLine("property float z");
            sw.WriteLine("element face {0}", triangles_array.Count / 3);
            sw.WriteLine("property list uchar int vertex_indices");
            sw.WriteLine("end_header");
            for (int i = 0; i < points_array.Count; i+=3)
            {
                sw.WriteLine("{0} {1} {2}", points_array[i], points_array[i + 1], points_array[i+2]);
            }
            for (int i = 0; i < triangles_array.Count; i+=3)
            {
                sw.WriteLine("3 {0} {1} {2}", triangles_array[i], triangles_array[i+1], triangles_array[i + 2]);
            }
            sw.Close();
        }
    }
}
