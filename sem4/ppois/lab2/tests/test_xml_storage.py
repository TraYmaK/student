from pathlib import Path

from student_registry.models import create_student_record
from student_registry.xml_storage import load_records_sax, save_records_dom


def test_xml_round_trip(tmp_path: Path) -> None:
    file_path = tmp_path / "students.xml"
    initial = [
        create_student_record(
            full_name="Sergey Volkov",
            course=4,
            group_name="P-41",
            total_works=14,
            completed_works=9,
            programming_language="Go",
        ),
        create_student_record(
            full_name="Olga Fedorova",
            course=1,
            group_name="P-11",
            total_works=8,
            completed_works=3,
            programming_language="C++",
        ),
    ]
    save_records_dom(initial, str(file_path))
    loaded = load_records_sax(str(file_path))
    assert len(loaded) == 2
    assert loaded[0].full_name == "Sergey Volkov"
    assert loaded[1].programming_language == "C++"
