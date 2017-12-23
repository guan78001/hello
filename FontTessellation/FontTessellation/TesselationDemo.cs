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
using System.Drawing;
using System.Windows.Forms;

namespace FontTessellation
{
    public partial class TesselationDemo : Form
    {
        string _currentFont;
        char _glyph;

        public TesselationDemo()
        {
            InitializeComponent();
            _glyph = 'A';
            RefreshFontList();

            lbFont.SelectedValueChanged += LbFontSelectedValueChanged;

            demo.DoTesselation(_currentFont, _glyph);
        }

        void LbFontSelectedValueChanged(object sender, EventArgs e)
        {
            _currentFont = (string) lbFont.SelectedItem;
            demo.DoTesselation(_currentFont, _glyph);
        }

        void RefreshFontList()
        {
            SuspendLayout();

            // We need a Graphics to get the list of font famillies
            using (var tmp = new Bitmap(10, 10))
            using (var gfx = Graphics.FromImage(tmp))
            {
                lbFont.Items.Clear();
                foreach (var family in FontFamily.GetFamilies(gfx))
                {
                    lbFont.Items.Add(family.Name);
                }
            }
            lbFont.SelectedItem = lbFont.Items[0];
            _currentFont = (string)lbFont.Items[0];

            ResumeLayout();
        }

        private void tbGlyph_Scroll(object sender, EventArgs e)
        {
            _glyph = Convert.ToChar(tbGlyph.Value);
            demo.DoTesselation(_currentFont, _glyph);
        }

        private void btnGO_Click(object sender, EventArgs e)
        {
            if(mtbGlyph.Text.Length==0) return;
            var glyph = mtbGlyph.Text[0];
            if(char.IsControl(glyph)) return;
            if (glyph >= 33 || glyph < 255) tbGlyph.Value = Convert.ToInt32(glyph);
            _glyph = glyph;
            demo.DoTesselation(_currentFont, _glyph);
        }
    }
}
