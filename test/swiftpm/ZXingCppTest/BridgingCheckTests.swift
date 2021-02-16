import XCTest
import CZXingCppTest

class BridgingCheckTests: XCTestCase {
    func testLoadValue() {
        XCTAssertEqual(CZXingCppTest.loadValue(), 0x03000102)
    }
}
