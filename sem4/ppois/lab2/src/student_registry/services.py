from dataclasses import dataclass

from student_registry.models import DomainError, FilterMode, SearchCriteria, StudentRecord


@dataclass(slots=True)
class Page:
    items: list[StudentRecord]
    total_items: int
    page: int
    total_pages: int
    per_page: int


class StudentRegistryService:
    def __init__(self) -> None:
        self._records: list[StudentRecord] = []

    def add_record(self, record: StudentRecord) -> None:
        self._records.append(record)

    def replace_records(self, records: list[StudentRecord]) -> None:
        self._records = list(records)

    def list_records(self) -> list[StudentRecord]:
        return list(self._records)

    def get_languages(self) -> list[str]:
        unique_languages = {record.programming_language for record in self._records}
        return sorted(unique_languages)

    def get_completed_values(self) -> list[int]:
        return sorted({record.completed_works for record in self._records})

    def get_total_values(self) -> list[int]:
        return sorted({record.total_works for record in self._records})

    def search(self, criteria: SearchCriteria) -> list[StudentRecord]:
        return [record for record in self._records if record_matches(record, criteria)]

    def delete(self, criteria: SearchCriteria) -> int:
        retained: list[StudentRecord] = []
        removed = 0
        for record in self._records:
            if record_matches(record, criteria):
                removed += 1
                continue
            retained.append(record)
        self._records = retained
        return removed


def paginate(records: list[StudentRecord], page: int, per_page: int) -> Page:
    if per_page <= 0:
        raise DomainError("Records per page must be positive.")
    total_items = len(records)
    total_pages = max(1, (total_items + per_page - 1) // per_page)
    normalized_page = min(max(1, page), total_pages)
    start = (normalized_page - 1) * per_page
    end = start + per_page
    return Page(
        items=records[start:end],
        total_items=total_items,
        page=normalized_page,
        total_pages=total_pages,
        per_page=per_page,
    )


def record_matches(record: StudentRecord, criteria: SearchCriteria) -> bool:
    if criteria.mode is FilterMode.BY_NAME_OR_GROUP:
        if criteria.text_query is None or not criteria.text_query.strip():
            raise DomainError("Search text must not be empty for name/group mode.")
        query = criteria.text_query.strip().lower()
        return query in record.full_name.lower() or query in record.group_name.lower()

    if criteria.mode is FilterMode.BY_COURSE_OR_LANGUAGE:
        if criteria.use_course is None:
            raise DomainError("Course/language selector is not set.")
        if criteria.use_course:
            if criteria.course_value is None:
                raise DomainError("Course value is required.")
            return record.course == criteria.course_value
        if criteria.language_value is None or not criteria.language_value.strip():
            raise DomainError("Language value is required.")
        return record.programming_language.lower() == criteria.language_value.strip().lower()

    if criteria.mode is FilterMode.BY_COMPLETED_OR_TOTAL:
        if criteria.use_completed is None:
            raise DomainError("Completed/total selector is not set.")
        if criteria.use_completed:
            if criteria.completed_value is None:
                raise DomainError("Completed value is required.")
            return record.completed_works == criteria.completed_value
        if criteria.total_value is None:
            raise DomainError("Total value is required.")
        return record.total_works == criteria.total_value

    if criteria.mode is FilterMode.BY_PENDING:
        if criteria.pending_value is None:
            raise DomainError("Pending value is required.")
        return record.pending_works == criteria.pending_value

    raise DomainError("Unsupported filter mode.")
