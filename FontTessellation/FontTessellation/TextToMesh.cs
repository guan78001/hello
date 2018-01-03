using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.IO;
using System.Drawing.Drawing2D;
using Poly2Tri;

namespace FontTessellation
{
    public partial class TextToMesh : Form
    {
        public TextToMesh()
        {
            InitializeComponent();
            this.comboBox1.TextChanged += comboBox1_TextChanged;
            RefreshFontList();
        }

        void comboBox1_TextChanged(object sender, EventArgs e)
        {
            string fontString = comboBox1.Text;
            this.textBox1.Font = new System.Drawing.Font(fontString, 36F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string text = this.textBox1.Text;
            string filename = "TextMesh.stl";
            //double zDepth =-10; // millimeter
            double zDepth = Convert.ToDouble(this.textBox2.Text);
            GenMesh(text, filename, zDepth);
            //this.Close();
        }
        private void GenMesh(string text,string filename,double zDepth)
        {
            Graphics g = this.CreateGraphics();
            var fx = g;

            //var ffamilly = new FontFamily("华文楷体");
            var ffamilly = new FontFamily(this.textBox1.Font.Name);
            FontStyle? fstyle = null;
            foreach (var style in new[] { FontStyle.Regular, FontStyle.Bold, FontStyle.Italic, FontStyle.Underline, FontStyle.Strikeout })
            {
                if (!ffamilly.IsStyleAvailable(style)) continue;
                fstyle = style;
                break;
            }
            if (!fstyle.HasValue) return;

            {
                List<List<PolygonPoint>> points_list = new List<List<PolygonPoint>>();
                var polygonListofText = GeneratePolygonsFromGlyph(fx, ffamilly, fstyle.Value, text, ref points_list);
                //WritePointsToFile(points_list, "d:\\temp\\points_list.txt");
                //WritePointsToBDMFile(points_list, "d:\\temp\\points_list.bdm");

                var polygonListofBase = Copy(polygonListofText);
                double[] min;
                double[] max;
                GetBoundingBox(ref points_list, out min, out max);
                polygonListofBase.Add(new Polygon(PolygonPointsOfRect(min, max)));

                var polygonSetOfText = CreateSetFromList(polygonListofText);
                var polygonSetOfBase = CreateSetFromList(polygonListofBase);

                P2T.Triangulate(polygonSetOfText);
                P2T.Triangulate(polygonSetOfBase);

                List<double> triangels = new List<double>();
                double z_text = zDepth;
                double z_base = 0;
                FromPolygonSetToTriangles(polygonSetOfText, z_text, triangels);
                FromPolygonSetToTriangles(polygonSetOfBase, z_base, triangels);
                GetTrianglesBetweenTwoZPlane(points_list, z_base, z_text, triangels);

                //reverse y
                for (int i = 1; i < triangels.Count; i += 3)
                {
                    triangels[i] = max[1] - triangels[i];
                }
                //WriteStl(triangels, filename);
                WriteStlBinary(triangels, filename);
            }
        }
        private void WritePointsToFile(List<List<PolygonPoint>> points_list,string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(points_list.Count);
            for (int i = 0; i < points_list.Count; i++)
            {
                sw.WriteLine(points_list[i].Count);
                for (int j = 0; j < points_list[i].Count; j++)
                {
                    sw.WriteLine("{0} {1}", points_list[i][j].X, points_list[i][j].Y);
                }
            }
            sw.Close();
        }
        private void WritePointsToBDMFile(List<List<PolygonPoint>> points_list, string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine("type \"2D\"");
            
            for (int i = 0; i < points_list.Count; i++)
            {
                sw.WriteLine(points_list[i].Count);
                for (int j = 0; j < points_list[i].Count; j++)
                {
                    sw.WriteLine("{0} {1}", points_list[i][j].X, points_list[i][j].Y);
                }
                sw.WriteLine();
            }
            sw.Close();
        }
        static List<Polygon> GeneratePolygonsFromGlyph(Graphics fx,
            FontFamily fontFamily, FontStyle style,
            string glyph, ref List<List<PolygonPoint>> points_list)
        {
            PointF[] pts = null;
            byte[] ptsType = null;
            float GlyphFontSize = 128;
            using (var path = new GraphicsPath())
            {
                path.AddString(glyph, fontFamily, (int)style, GlyphFontSize,
                    new PointF(0f, 0f), StringFormat.GenericDefault);

                path.Flatten();

                if (path.PointCount == 0) return new List<Polygon>();

                pts = path.PathPoints;
                ptsType = path.PathTypes;
            }

            var polygons = new List<Polygon>();
            List<PolygonPoint> points = null;
            Pen cPen = Pens.Yellow;
            var start = -1;

            for (var i = 0; i < pts.Length; i++)
            {
                var pointType = ptsType[i] & 0x07;
                if (pointType == 0)
                {
                    points = new List<PolygonPoint> { new PolygonPoint(pts[i].X, pts[i].Y) };
                    start = i;
                    continue;
                }
                if (pointType != 1) throw new Exception("Unsupported point type");


                if ((ptsType[i] & 0x80) != 0)
                {
                    //- Last point in the polygon
                    if (pts[i] != pts[start])
                    {
                        points.Add(new PolygonPoint(pts[i].X, pts[i].Y));
                    }
                    polygons.Add(new Polygon(points));
                    points_list.Add(points);
                    points = null;
                }
                else
                {
                    points.Add(new PolygonPoint(pts[i].X, pts[i].Y));
                }
            }
            return polygons;
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
            polygonList_base.Add(new Polygon(PolygonPointsOfRect(min, max)));
            return polygonList_base;
        }
        private List<Polygon> Copy(List<Polygon> polygonList_bottom)
        {
            var polygonList_base = new List<Polygon>();
            for (int i = 0; i < polygonList_bottom.Count; i++)
            {
                polygonList_base.Add(polygonList_bottom[i]);
            }
            return polygonList_base;
        }
        private List<PolygonPoint> PolygonPointsOfRect(double[] min, double[] max)
        {
            //double x1 = Math.Max(0, min[0] - 10), y1 = Math.Max(0, min[1] - 10), x2 = max[0] + 10, y2 = max[1] + 10;
            double x1 = min[0] - 10, y1 = min[1] - 10, x2 = max[0] + 10, y2 = max[1] + 10;
            //double x1 = 0, y1 = 0, x2 = 650, y2 = 350;
            var points = new List<PolygonPoint>();
            points.Add(new PolygonPoint(x1, y1));
            points.Add(new PolygonPoint(x2, y1));
            points.Add(new PolygonPoint(x2, y2));
            points.Add(new PolygonPoint(x1, y2));

            return points;
        }
        private void FromPolygonSetToTriangles(PolygonSet polygonSet2, double z, List<double> triangels)
        {
            foreach (var pol in polygonSet2.Polygons)
            {
                foreach (var tri in pol.Triangles)
                {
                    //change orientation due to y-axis was reversed from text to mesh coordinate
                    triangels.Add(tri.Points[0].X);
                    triangels.Add(tri.Points[0].Y);
                    triangels.Add(z);

                    triangels.Add(tri.Points[2].X);
                    triangels.Add(tri.Points[2].Y);
                    triangels.Add(z);

                    triangels.Add(tri.Points[1].X);
                    triangels.Add(tri.Points[1].Y);
                    triangels.Add(z);
                }
            }
        }
        private void WriteStl(List<double> triangles_xyz_array, string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine("solid STL generated by GuanMenghui");

            for (int i = 0; i < triangles_xyz_array.Count; i += 9)
            {
                sw.WriteLine("  facet normal 0.000000e+000 0.000000e+000 1.000000e+000");
                sw.WriteLine("    outer loop");

                for (int j = 0; j < 3; j++)
                {
                    sw.WriteLine("      vertex  {0:e} {1:e} {2:e}", triangles_xyz_array[i + 3 * j], triangles_xyz_array[i + 3 * j + 1], triangles_xyz_array[i + 3 * j + 2]);
                }
                sw.WriteLine("    endloop");
                sw.WriteLine("  endfacet");
            }
            sw.WriteLine("endsolid stl");
            sw.Close();
        }
        private void WriteStlBinary(List<double> triangles_xyz_array, string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Create);
            BinaryWriter bw = new BinaryWriter(fs);
            byte[]header=new byte[80];
            UInt32 nTriangels = Convert.ToUInt32(triangles_xyz_array.Count);

            bw.Write(header);
            bw.Write(nTriangels);
            float []normal=new float[3]{0,0,1};
            for(int i=0; i<triangles_xyz_array.Count; i+=9){
                bw.Write(normal[0]);
                bw.Write(normal[1]);
                bw.Write(normal[2]);
                for (int vId = 0; vId < 3; vId++)
                {
                    for (int offset = 0; offset < 3; offset++ )
                        bw.Write(Convert.ToSingle(triangles_xyz_array[i + 3 * vId + offset]));
                }
                UInt16 attribute = 0;
                bw.Write(attribute);
            }
            bw.Write("end");
            bw.Close();
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
                else { p1 = newPoint; p2 = oldPoint; }
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

        static void ProcessLevel(Polygon poly, ref PolygonHierachy localRoot)
        {
            if (localRoot == null)
            {
                localRoot = new PolygonHierachy(poly);
                return;
            }

            // Check if source is the new root
            if (CheckIfInside(localRoot.Current.Points, poly.Points))
            {
                var nroot = new PolygonHierachy(poly);
                var tmp = localRoot;
                while (tmp != null)
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
            for (var i = 0; i < localRoot.Childs.Count; ++i)
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
                if (CheckIfInside(localRoot.Childs[i].Current.Points, poly.Points))
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

        static void ProcessSetLevel(PolygonSet set, PolygonHierachy current)
        {
            while (current != null)
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

            for (var i = 1; i < source.Count; ++i)
            {
                ProcessLevel(source[i], ref root);
            }

            // Generate the set from the hierachy
            var set = new PolygonSet();
            ProcessSetLevel(set, root);

            return set;
        }


        private void GetTrianglesBetweenTwoZPlane(List<List<PolygonPoint>> points_list, double z1, double z2, List<double> triangles_xyz_array)
        {
            List<double> points_array = new List<double>();
            List<int> triangles_array = new List<int>();
            for (int i = 0; i < points_list.Count; i++)
            {
                int idStart = points_array.Count / 3;
                for (int j = 0; j < points_list[i].Count; j++)
                {
                    double x = (points_list[i][j].X);
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
                for (int id = idStart; id < idEnd - 2; id += 2)
                {
                    //Add two triangles
                    int[] arr = new int[6] { id, id + 1, id + 3, id, id + 3, id + 2 };
                    //int[] arr = new int[6] { id, id + 3, id + 1, id, id + 2, id + 3};
                    AddTriangles(ref triangles_xyz_array, points_array, arr);
                }
                //Connect idStart and idEnd, add two triangles
                int[] arr_last = new int[6] { idEnd - 2, idEnd - 1, idStart + 1, idEnd - 2, idStart + 1, idStart };
                //int[] arr_last = new int[6] { idEnd - 2, idStart + 1, idEnd - 1, idEnd - 2, idStart, idStart + 1 };
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

        void RefreshFontList()
        {
            SuspendLayout();

            // We need a Graphics to get the list of font famillies
            using (var tmp = new Bitmap(10, 10))
            using (var gfx = Graphics.FromImage(tmp))
            {
                this.comboBox1.Items.Clear();
                foreach (var family in FontFamily.GetFamilies(gfx))
                {
                    comboBox1.Items.Add(family.Name);
                }
            }
            comboBox1.SelectedIndex=0;
            string fontString = comboBox1.Items[0].ToString();
            this.textBox1.Font = new System.Drawing.Font(fontString, 36F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            ResumeLayout();
        }
    }

}
