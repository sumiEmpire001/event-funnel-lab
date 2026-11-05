import tempfile
import unittest
from pathlib import Path
from funnel import analyze,run


def event(user,name,time):
    return {"user_id":user,"event":name,"timestamp":"2026-11-05T10:0"+str(time)+":00Z"}


class FunnelTests(unittest.TestCase):
    def test_complete(self):
        result=analyze([event("a","view",0),event("a","signup",1)],["view","signup"])
        self.assertEqual(result["steps"][1]["users"],1)
    def test_order_matters(self):
        result=analyze([event("a","signup",0),event("a","view",1)],["view","signup"])
        self.assertEqual(result["steps"][1]["users"],0)
    def test_multiple_users(self):
        result=analyze([event("a","view",0),event("b","view",0)],["view"])
        self.assertEqual(result["steps"][0]["users"],2)
    def test_duplicate_events(self):
        result=analyze([event("a","view",0),event("a","view",1)],["view"])
        self.assertEqual(result["steps"][0]["users"],1)
    def test_no_first_stage(self):
        self.assertIsNone(analyze([event("a","signup",0)],["view","signup"])["steps"][1]["conversion_from_first"])
    def test_duplicate_steps(self):
        with self.assertRaises(ValueError): analyze([],["view","view"])
    def test_bad_timestamp(self):
        with self.assertRaises(ValueError): analyze([{"user_id":"a","event":"view","timestamp":"2026-11-05T10:00:00"}],["view"])
    def test_csv(self):
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/"events.csv"
            path.write_text("user_id,event,timestamp\na,view,2026-11-05T10:00:00Z\n",encoding="utf-8")
            self.assertEqual(run(str(path),["view"])["users"],1)


if __name__=="__main__":
    unittest.main()
