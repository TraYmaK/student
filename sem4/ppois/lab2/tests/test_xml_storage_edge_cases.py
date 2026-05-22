from pathlib import Path

import pytest

from student_registry.models import DomainError
from student_registry.xml_storage import load_records_sax, save_records_dom


def test_save_empty_records_and_load_back(tmp_path: Path) -> None:
    file_path = tmp_path / "empty.xml"
    save_records_dom([], str(file_path))
    loaded = load_records_sax(str(file_path))
    assert loaded == []


def test_load_invalid_xml_raises_domain_error(tmp_path: Path) -> None:
    file_path = tmp_path / "broken.xml"
    file_path.write_text(
        "<students><student><full_name>Only Name</full_name></student></students>",
        encoding="utf-8",
    )
    with pytest.raises(DomainError):
        load_records_sax(str(file_path))
