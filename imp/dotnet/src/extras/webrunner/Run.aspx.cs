using System;
using System.IO;
using System.Net;
using System.Reflection;
using System.Web.UI;
using System.Web.UI.WebControls;

using fit;
using eg;

namespace Extras.WebRunner
{
	public class Run : Page
	{
		protected Literal output;

		public Run() 
		{
            // eh?
			string fitPath = new FileInfo(typeof(fit.Fixture).Assembly.Location).Directory.ToString();
			string egPath = new FileInfo(typeof(eg.ArithmeticFixture).Assembly.Location).Directory.ToString();
			string binPath = Server.MapPath("") + @"\bin";
			Fixture.assemblyDirs = new string [] {fitPath, egPath, binPath};
		}

        protected override void OnLoad(EventArgs e) {
            base.OnLoad (e);
            output.Text = TestOutput();
        }

        private string TestOutput() {
			if (Request.UrlReferrer == null) return "couldn't find referring page";

            Parse tables = new Parse(ReadContents(Request.UrlReferrer));
            new Fixture().doTables(tables);
            using (StringWriter writer = new StringWriter()) {
                tables.print(writer);
                return writer.ToString();
            }
		}
    
        private string ReadContents(Uri uri) {
            try {
                HttpWebRequest request = (HttpWebRequest) WebRequest.Create(uri);
                using (WebResponse response = request.GetResponse()) {
                    return new StreamReader(response.GetResponseStream()).ReadToEnd();
                }
            }
            catch (WebException e) {
                throw new ApplicationException("Couldn't open " + uri, e);
            }
        }
    }
}
