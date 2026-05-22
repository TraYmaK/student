from dataclasses import dataclass
from enum import Enum


class DomainError(ValueError):
    """Base error for invalid domain states."""


class FilterMode(str, Enum):
    BY_NAME_OR_GROUP = "by_name_or_group"
    BY_COURSE_OR_LANGUAGE = "by_course_or_language"
    BY_COMPLETED_OR_TOTAL = "by_completed_or_total"
    BY_PENDING = "by_pending"


@dataclass(slots=True)
class StudentRecord:
    full_name: str
    course: int
    group_name: str
    total_works: int
    completed_works: int
    programming_language: str

    @property
    def pending_works(self) -> int:
        return self.total_works - self.completed_works


@dataclass(slots=True)
class SearchCriteria:
    mode: FilterMode
    text_query: str | None = None
    use_course: bool | None = None
    course_value: int | None = None
    language_value: str | None = None
    use_completed: bool | None = None
    completed_value: int | None = None
    total_value: int | None = None
    pending_value: int | None = None


def create_student_record(
    full_name: str,
    course: int,
    group_name: str,
    total_works: int,
    completed_works: int,
    programming_language: str,
) -> StudentRecord:
    normalized_name = full_name.strip()
    normalized_group = group_name.strip()
    normalized_language = programming_language.strip()
    if not normalized_name:
        raise DomainError("Student full name must not be empty.")
    if not normalized_group:
        raise DomainError("Group must not be empty.")
    if not normalized_language:
        raise DomainError("Programming language must not be empty.")
    if course < 1 or course > 6:
        raise DomainError("Course must be between 1 and 6.")
    if total_works < 0:
        raise DomainError("Total works must not be negative.")
    if completed_works < 0:
        raise DomainError("Completed works must not be negative.")
    if completed_works > total_works:
        raise DomainError("Completed works must be less than or equal to total works.")
    return StudentRecord(
        full_name=normalized_name,
        course=course,
        group_name=normalized_group,
        total_works=total_works,
        completed_works=completed_works,
        programming_language=normalized_language,
    )
