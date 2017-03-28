package net.sf.sevenzipjbinding.junit.jbindingtools;

import static org.junit.Assert.assertNull;

import org.junit.Rule;
import org.junit.Test;

import net.sf.sevenzipjbinding.junit.DebugModeOnly;
import net.sf.sevenzipjbinding.junit.DebugModeOnlyTestRule;
import net.sf.sevenzipjbinding.junit.JUnitNativeTestBase;

public class CHeadCacheInStreamTest extends JUnitNativeTestBase {
    @Rule
    public DebugModeOnlyTestRule debugModeOnlyTestRule = new DebugModeOnlyTestRule();

    private native String nativeSimpleTests();

    @Test
    @DebugModeOnly
    public void simpleTests() {
        String errors = nativeSimpleTests();
        assertNull(errors, errors);
    }
}


