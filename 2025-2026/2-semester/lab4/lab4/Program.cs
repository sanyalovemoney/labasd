using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Windows.Forms;

namespace GraphLab4
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.SetHighDpiMode(HighDpiMode.SystemAware);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }

    public partial class Form1 : Form
    {
        const int n1 = 3;
        const int n2 = 4;
        const int n3 = 1;
        const int n4 = 2;                                       

        private int N;
        private int[,] matrixDir;
        private int[,] matrixUndir;
        private int[,] matrixCondensation;
        private List<List<int>> sccComponents;

        private RichTextBox txtOutput;
        private PictureBox pbGraph;
        private Button btnTask1_2;
        private Button btnTask3_4;
        private bool showCondensation = false;

        public Form1()
        {
            InitializeComponentCustom();
            N = 10 + n3;
        }

        private void RunTask1_2(object sender, EventArgs e)
        {
            showCondensation = false;
            txtOutput.Clear();
            txtOutput.AppendText($"=== TASK 1 & 2 ===\nVariant: {n1}{n2}{n3}{n4}, N={N}\n");

            double k = 1.0 - (n3 * 0.01) - (n4 * 0.01) - 0.3;
            txtOutput.AppendText($"Coefficient K = {k:F3}\n");

            matrixDir = GenerateMatrix(k);
            matrixUndir = SymmetrizeMatrix(matrixDir);

            PrintMatrix("Adjacency Matrix (Directed):", matrixDir);
            PrintMatrix("Adjacency Matrix (Undirected):", matrixUndir);

            CalculateDegrees(matrixDir, matrixUndir);
            pbGraph.Invalidate();
        }

        private void RunTask3_4(object sender, EventArgs e)
        {
            showCondensation = false;
            txtOutput.Clear();
            txtOutput.AppendText($"=== TASK 3 & 4 ===\n");

            double k = 1.0 - (n3 * 0.005) - (n4 * 0.005) - 0.27;
            txtOutput.AppendText($"New coefficient K = {k:F3}\n");

            matrixDir = GenerateMatrix(k);
            PrintMatrix("New Adjacency Matrix:", matrixDir);

            CalculateDegreesDirectedOnly(matrixDir);
            FindPaths(matrixDir, 2);
            FindPaths(matrixDir, 3);

            int[,] reachability = GetReachabilityMatrix(matrixDir);
            PrintMatrix("Reachability Matrix:", reachability);

            int[,] strongConn = GetStrongConnectivityMatrix(reachability);
            PrintMatrix("Strong Connectivity Matrix:", strongConn);

            sccComponents = FindComponents(strongConn);
            txtOutput.AppendText("\nStrongly Connected Components:\n");
            for (int i = 0; i < sccComponents.Count; i++)
            {
                txtOutput.AppendText($"K{i + 1}: {{ {string.Join(", ", sccComponents[i].Select(v => v + 1))} }}\n");
            }

            matrixCondensation = BuildCondensationMatrix(matrixDir, sccComponents);
            PrintMatrix("Condensation Graph Matrix:", matrixCondensation);

            showCondensation = true;
            pbGraph.Invalidate();
            MessageBox.Show("The condensation graph is displayed in the graphics window.");
        }

        private int[,] GenerateMatrix(double k)
        {
            int seed = int.Parse($"{n1}{n2}{n3}{n4}");
            Random rand = new Random(seed);
            int[,] mat = new int[N, N];

            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    double val = rand.NextDouble() * 2.0;
                    val = val * k;
                    mat[i, j] = (val >= 1.0) ? 1 : 0;
                }
            }
            return mat;
        }

        private int[,] SymmetrizeMatrix(int[,] mat)
        {
            int[,] res = new int[N, N];
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    if (mat[i, j] == 1 || mat[j, i] == 1) res[i, j] = 1;
                    else res[i, j] = 0;
                }
            }
            return res;
        }

        private void CalculateDegrees(int[,] dir, int[,] undir)
        {
            txtOutput.AppendText("\n--- Graph Characteristics ---\n");
            txtOutput.AppendText("Undirected graph:\n");
            int[] deg = new int[N];
            bool regular = true;
            for (int i = 0; i < N; i++)
            {
                deg[i] = GetRowSum(undir, i);
                txtOutput.AppendText($"V{i + 1}: deg={deg[i]}\n");
                if (i > 0 && deg[i] != deg[i - 1]) regular = false;
            }

            if (regular) txtOutput.AppendText($"Graph is regular, degree {deg[0]}.\n");
            else txtOutput.AppendText("Graph is not regular.\n");

            txtOutput.AppendText("\nDirected graph:\n");
            List<int> isolated = new List<int>();
            List<int> hanging = new List<int>();

            for (int i = 0; i < N; i++)
            {
                int degOut = GetRowSum(dir, i);
                int degIn = GetColSum(dir, i);
                txtOutput.AppendText($"V{i + 1}: out={degOut}, in={degIn}\n");

                if (degOut == 0 && degIn == 0) isolated.Add(i + 1);
                if ((degOut == 1 && degIn == 0) || (degOut == 0 && degIn == 1)) hanging.Add(i + 1);
            }
            txtOutput.AppendText($"Isolated vertices: {(isolated.Count > 0 ? string.Join(",", isolated) : "none")}\n");
            txtOutput.AppendText($"Pendant vertices: {(hanging.Count > 0 ? string.Join(",", hanging) : "none")}\n");
        }

        private void CalculateDegreesDirectedOnly(int[,] mat)
        {
            txtOutput.AppendText("\nVertex semi-degrees (out/in):\n");
            for (int i = 0; i < N; i++)
                txtOutput.AppendText($"V{i + 1}: out={GetRowSum(mat, i)}, in={GetColSum(mat, i)}\n");
        }

        private void FindPaths(int[,] mat, int len)
        {
            txtOutput.AppendText($"\n--- Paths of length {len} ---\n");
            if (len == 2)
            {
                for (int i = 0; i < N; i++)
                    for (int j = 0; j < N; j++)
                        for (int k = 0; k < N; k++)
                            if (mat[i, k] == 1 && mat[k, j] == 1)
                                txtOutput.AppendText($"{i + 1} -> {k + 1} -> {j + 1}\n");
            }
            else if (len == 3)
            {
                for (int i = 0; i < N; i++)
                    for (int j = 0; j < N; j++)
                        for (int k = 0; k < N; k++)
                            if (mat[i, k] == 1)
                                for (int m = 0; m < N; m++)
                                    if (mat[k, m] == 1 && mat[m, j] == 1)
                                        txtOutput.AppendText($"{i + 1} -> {k + 1} -> {m + 1} -> {j + 1}\n");
            }
        }

        private int[,] GetReachabilityMatrix(int[,] adj)
        {
            int[,] R = (int[,])adj.Clone();
            for (int i = 0; i < N; i++) R[i, i] = 1;
            for (int k = 0; k < N; k++)
                for (int i = 0; i < N; i++)
                    for (int j = 0; j < N; j++)
                        if (R[i, k] == 1 && R[k, j] == 1) R[i, j] = 1;
            return R;
        }

        private int[,] GetStrongConnectivityMatrix(int[,] R)
        {
            int[,] S = new int[N, N];
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    if (R[i, j] == 1 && R[j, i] == 1) S[i, j] = 1;
                    else S[i, j] = 0;
            return S;
        }

        private List<List<int>> FindComponents(int[,] S)
        {
            bool[] visited = new bool[N];
            List<List<int>> components = new List<List<int>>();
            for (int i = 0; i < N; i++)
            {
                if (!visited[i])
                {
                    List<int> comp = new List<int>();
                    for (int j = 0; j < N; j++)
                    {
                        if (S[i, j] == 1)
                        {
                            comp.Add(j);
                            visited[j] = true;
                        }
                    }
                    components.Add(comp);
                }
            }
            return components;
        }

        private int[,] BuildCondensationMatrix(int[,] adj, List<List<int>> comps)
        {
            int size = comps.Count;
            int[,] cond = new int[size, size];
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (i == j) continue;
                    bool edgeExists = false;
                    foreach (int u in comps[i])
                    {
                        foreach (int v in comps[j])
                        {
                            if (adj[u, v] == 1) { edgeExists = true; break; }
                        }
                        if (edgeExists) break;
                    }
                    cond[i, j] = edgeExists ? 1 : 0;
                }
            }
            return cond;
        }

        private int GetRowSum(int[,] mat, int row)
        {
            int sum = 0;
            for (int j = 0; j < N; j++) sum += mat[row, j];
            return sum;
        }
        private int GetColSum(int[,] mat, int col)
        {
            int sum = 0;
            for (int i = 0; i < N; i++) sum += mat[i, col];
            return sum;
        }
        private void PrintMatrix(string title, int[,] mat)
        {
            txtOutput.AppendText($"\n{title}\n");
            int rows = mat.GetLength(0);
            int cols = mat.GetLength(1);
            txtOutput.AppendText("   ");
            for (int j = 0; j < cols; j++) txtOutput.AppendText($"{j + 1,2} ");
            txtOutput.AppendText("\n");
            for (int i = 0; i < rows; i++)
            {
                txtOutput.AppendText($"{i + 1,2}|");
                for (int j = 0; j < cols; j++) txtOutput.AppendText($"{mat[i, j],2} ");
                txtOutput.AppendText("\n");
            }
        }

        private void pbGraph_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            if (matrixDir == null && !showCondensation) return;

            int w = pbGraph.Width;
            int h = pbGraph.Height;
            int centerX = w / 2;
            int centerY = h / 2;
            int radius = Math.Min(w, h) / 2 - 40;

            if (!showCondensation)
            {
                PointF[] points = GetCircleCoords(N, centerX, centerY, radius);
                Pen p = new Pen(Color.Black, 1);
                p.CustomEndCap = new System.Drawing.Drawing2D.AdjustableArrowCap(4, 4);

                for (int i = 0; i < N; i++)
                    for (int j = 0; j < N; j++)
                        if (matrixDir[i, j] == 1)
                        {
                            if (i == j) DrawLoop(e.Graphics, points[i]);
                            else DrawArrow(e.Graphics, p, points[i], points[j]);
                        }
                DrawNodes(e.Graphics, points, Enumerable.Range(1, N).Select(x => x.ToString()).ToArray());
            }
            else
            {
                if (matrixCondensation == null) return;
                int count = matrixCondensation.GetLength(0);
                PointF[] points = GetCircleCoords(count, centerX, centerY, radius);
                Pen p = new Pen(Color.Blue, 2);
                p.CustomEndCap = new System.Drawing.Drawing2D.AdjustableArrowCap(5, 5);

                for (int i = 0; i < count; i++)
                    for (int j = 0; j < count; j++)
                        if (matrixCondensation[i, j] == 1)
                        {
                            if (i == j) DrawLoop(e.Graphics, points[i]);
                            else DrawArrow(e.Graphics, p, points[i], points[j]);
                        }

                string[] labels = new string[count];
                for (int i = 0; i < count; i++) labels[i] = "K" + (i + 1);
                DrawNodes(e.Graphics, points, labels, Brushes.LightGreen);
                e.Graphics.DrawString("Condensation Graph", new Font("Arial", 14, FontStyle.Bold), Brushes.Blue, 10, 10);
            }
        }

        private PointF[] GetCircleCoords(int count, int cx, int cy, int r)
        {
            PointF[] pts = new PointF[count];
            double angleStep = 2 * Math.PI / count;
            for (int i = 0; i < count; i++)
                pts[i] = new PointF((float)(cx + r * Math.Cos(i * angleStep - Math.PI / 2)), (float)(cy + r * Math.Sin(i * angleStep - Math.PI / 2)));
            return pts;
        }

        private void DrawNodes(Graphics g, PointF[] pts, string[] labels, Brush bg = null)
        {
            if (bg == null) bg = Brushes.White;
            int r = 15;
            Font f = new Font("Arial", 10);
            StringFormat sf = new StringFormat() { Alignment = StringAlignment.Center, LineAlignment = StringAlignment.Center };
            for (int i = 0; i < pts.Length; i++)
            {
                RectangleF rect = new RectangleF(pts[i].X - r, pts[i].Y - r, 2 * r, 2 * r);
                g.FillEllipse(bg, rect);
                g.DrawEllipse(Pens.Black, rect);
                g.DrawString(labels[i], f, Brushes.Black, rect, sf);
            }
        }

        private void DrawArrow(Graphics g, Pen p, PointF p1, PointF p2)
        {
            float r = 15;
            double angle = Math.Atan2(p2.Y - p1.Y, p2.X - p1.X);
            PointF start = new PointF((float)(p1.X + r * Math.Cos(angle)), (float)(p1.Y + r * Math.Sin(angle)));
            PointF end = new PointF((float)(p2.X - r * Math.Cos(angle)), (float)(p2.Y - r * Math.Sin(angle)));
            g.DrawLine(p, start, end);
        }

        private void DrawLoop(Graphics g, PointF p)
        {
            int r = 15;
            g.DrawEllipse(Pens.Black, p.X - r, p.Y - r * 2 - 5, 20, 20);
        }

        private void InitializeComponentCustom()
        {
            this.Size = new Size(1000, 700);
            this.Text = "Graph Theory Lab";
            pbGraph = new PictureBox();
            pbGraph.Location = new Point(10, 10);
            pbGraph.Size = new Size(600, 640);
            pbGraph.BorderStyle = BorderStyle.FixedSingle;
            pbGraph.BackColor = Color.White;
            pbGraph.Paint += pbGraph_Paint;
            this.Controls.Add(pbGraph);
            txtOutput = new RichTextBox();
            txtOutput.Location = new Point(620, 10);
            txtOutput.Size = new Size(350, 550);
            txtOutput.Font = new Font("Consolas", 9);
            this.Controls.Add(txtOutput);
            btnTask1_2 = new Button();
            btnTask1_2.Text = "Task 1-2";
            btnTask1_2.Location = new Point(620, 570);
            btnTask1_2.Size = new Size(170, 40);
            btnTask1_2.Click += RunTask1_2;
            this.Controls.Add(btnTask1_2);
            btnTask3_4 = new Button();
            btnTask3_4.Text = "Task 3-4 (Condensation)";
            btnTask3_4.Location = new Point(800, 570);
            btnTask3_4.Size = new Size(170, 40);
            btnTask3_4.Click += RunTask3_4;
            this.Controls.Add(btnTask3_4);
        }
    }
}