using NUnit.Framework;
using fit;

[TestFixture]
public class FixtureTest {

	[Test]
	public void testEscape() {
		Assert.AreEqual(" &nbsp; &nbsp; ", Fixture.escape("     "));
	}
}
