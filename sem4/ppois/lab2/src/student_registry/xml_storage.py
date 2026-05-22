from pathlib import Path
from xml.dom.minidom import Document
from xml.sax import ContentHandler, parse

from student_registry.models import StudentRecord, create_student_record


def save_records_dom(records: list[StudentRecord], file_path: str) -> None:
    document = Document()
    root = document.createElement("students")
    document.appendChild(root)
    for record in records:
        student_node = document.createElement("student")
        root.appendChild(student_node)
        _append_text_node(document, student_node, "full_name", record.full_name)
        _append_text_node(document, student_node, "course", str(record.course))
        _append_text_node(document, student_node, "group_name", record.group_name)
        _append_text_node(document, student_node, "total_works", str(record.total_works))
        _append_text_node(document, student_node, "completed_works", str(record.completed_works))
        _append_text_node(
            document,
            student_node,
            "programming_language",
            record.programming_language,
        )
    xml_text = document.toprettyxml(indent="  ", encoding="utf-8")
    Path(file_path).write_bytes(xml_text)


def load_records_sax(file_path: str) -> list[StudentRecord]:
    handler = _StudentsHandler()
    parse(file_path, handler)
    return handler.records


def _append_text_node(document: Document, parent, name: str, value: str) -> None:
    node = document.createElement(name)
    parent.appendChild(node)
    node.appendChild(document.createTextNode(value))


class _StudentsHandler(ContentHandler):
    def __init__(self) -> None:
        super().__init__()
        self.records: list[StudentRecord] = []
        self._current_tag = ""
        self._current_data: dict[str, str] = {}
        self._text_buffer: list[str] = []

    def startElement(self, name: str, attrs) -> None:
        if name == "student":
            self._current_data = {}
        self._current_tag = name
        self._text_buffer = []

    def characters(self, content: str) -> None:
        self._text_buffer.append(content)

    def endElement(self, name: str) -> None:
        text_value = "".join(self._text_buffer).strip()
        if name in {
            "full_name",
            "course",
            "group_name",
            "total_works",
            "completed_works",
            "programming_language",
        }:
            self._current_data[name] = text_value
        if name == "student":
            self.records.append(
                create_student_record(
                    full_name=self._current_data.get("full_name", ""),
                    course=int(self._current_data.get("course", "0")),
                    group_name=self._current_data.get("group_name", ""),
                    total_works=int(self._current_data.get("total_works", "0")),
                    completed_works=int(self._current_data.get("completed_works", "0")),
                    programming_language=self._current_data.get("programming_language", ""),
                )
            )
            self._current_data = {}
        self._current_tag = ""
        self._text_buffer = []
