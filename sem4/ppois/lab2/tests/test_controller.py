import importlib
import sys
import types

import pytest

from student_registry.models import FilterMode, SearchCriteria, create_student_record
from student_registry.services import StudentRegistryService


class FakeMainWindow:
    def __init__(
        self,
        on_add,
        on_search,
        on_delete,
        on_load,
        on_save,
        on_navigate,
        on_per_page_changed,
    ) -> None:
        self.on_add = on_add
        self.on_search = on_search
        self.on_delete = on_delete
        self.on_load = on_load
        self.on_save = on_save
        self.on_navigate = on_navigate
        self.on_per_page_changed = on_per_page_changed
        self.page_history = []
        self.errors: list[str] = []
        self.infos: list[str] = []
        self.open_file = ""
        self.save_file = ""
        self.mainloop_calls = 0

    def set_page(self, page_data) -> None:
        self.page_history.append(page_data)

    def mainloop(self) -> None:
        self.mainloop_calls += 1

    def show_error(self, message: str) -> None:
        self.errors.append(message)

    def show_info(self, message: str) -> None:
        self.infos.append(message)

    def ask_open_file(self) -> str:
        return self.open_file

    def ask_save_file(self) -> str:
        return self.save_file


class FakeAddDialog:
    auto_payload: dict[str, str] | None = None

    def __init__(self, parent, on_submit, available_languages) -> None:
        self.on_submit = on_submit
        self.available_languages = available_languages
        self.destroyed = False

    def wait_window(self) -> None:
        if self.auto_payload is not None:
            self.on_submit(self.auto_payload)

    def destroy(self) -> None:
        self.destroyed = True


class FakeSearchDialog:
    exists: bool = True

    def __init__(self, parent, on_search, on_reset, on_navigate, on_per_page_changed) -> None:
        self.on_search = on_search
        self.on_reset = on_reset
        self.on_navigate = on_navigate
        self.on_per_page_changed = on_per_page_changed
        self.focus_calls = 0
        self.dropdowns = None
        self.page_history = []
        self.errors: list[str] = []

    def winfo_exists(self) -> bool:
        return self.exists

    def focus(self) -> None:
        self.focus_calls += 1

    def set_dropdown_values(self, languages, completed_values, total_values) -> None:
        self.dropdowns = (languages, completed_values, total_values)

    def set_page(self, page_data) -> None:
        self.page_history.append(page_data)

    def show_error(self, message: str) -> None:
        self.errors.append(message)


class FakeDeleteDialog:
    auto_criteria: SearchCriteria | None = None

    def __init__(self, parent, on_delete) -> None:
        self.on_delete = on_delete
        self.dropdowns = None
        self.destroyed = False

    def set_dropdown_values(self, languages, completed_values, total_values) -> None:
        self.dropdowns = (languages, completed_values, total_values)

    def wait_window(self) -> None:
        if self.auto_criteria is not None:
            self.on_delete(self.auto_criteria)

    def destroy(self) -> None:
        self.destroyed = True


@pytest.fixture()
def patched_controller(monkeypatch):
    fake_view = types.ModuleType("student_registry.view")
    fake_view.MainWindow = FakeMainWindow
    fake_view.AddRecordDialog = FakeAddDialog
    fake_view.SearchDialog = FakeSearchDialog
    fake_view.DeleteDialog = FakeDeleteDialog
    monkeypatch.setitem(sys.modules, "student_registry.view", fake_view)
    controller_module = importlib.import_module("student_registry.controller")
    controller_module = importlib.reload(controller_module)
    service = StudentRegistryService()
    service.add_record(
        create_student_record(
            full_name="Ivan Petrov",
            course=2,
            group_name="P-21",
            total_works=12,
            completed_works=8,
            programming_language="Python",
        )
    )
    return controller_module, controller_module.AppController(service)


def test_run_calls_mainloop_and_sets_first_page(patched_controller) -> None:
    _, patched_controller = patched_controller
    patched_controller.run()
    window = patched_controller._main_window
    assert window.mainloop_calls == 1
    assert len(window.page_history) >= 1


def test_navigate_main_and_set_per_page(patched_controller) -> None:
    _, patched_controller = patched_controller
    patched_controller.set_main_per_page(5)
    patched_controller.navigate_main("next")
    patched_controller.navigate_main("last")
    assert patched_controller._main_per_page == 5
    assert len(patched_controller._main_window.page_history) >= 3


def test_open_add_dialog_success_and_validation_error(patched_controller) -> None:
    _, patched_controller = patched_controller
    FakeAddDialog.auto_payload = {
        "full_name": "Anna Smirnova",
        "course": "3",
        "group_name": "P-31",
        "total_works": "10",
        "completed_works": "6",
        "programming_language": "Java",
    }
    patched_controller.open_add_dialog()
    assert len(patched_controller._service.list_records()) == 2

    FakeAddDialog.auto_payload = {
        "full_name": "Bad Record",
        "course": "2",
        "group_name": "P-22",
        "total_works": "3",
        "completed_works": "9",
        "programming_language": "Python",
    }
    patched_controller.open_add_dialog()
    assert patched_controller._main_window.errors


def test_open_search_dialog_focus_existing_dialog(patched_controller) -> None:
    _, patched_controller = patched_controller
    patched_controller.open_search_dialog()
    search_dialog = patched_controller._search_dialog
    patched_controller.open_search_dialog()
    assert search_dialog.focus_calls == 1


def test_search_reset_refresh_and_navigation(patched_controller) -> None:
    _, patched_controller = patched_controller
    patched_controller.open_search_dialog()
    patched_controller.search_records(
        SearchCriteria(mode=FilterMode.BY_NAME_OR_GROUP, text_query="Ivan")
    )
    patched_controller.set_search_per_page(2)
    patched_controller.navigate_search("next")
    patched_controller.reset_search_results()
    assert patched_controller._search_dialog.page_history


def test_search_domain_error_is_shown(patched_controller) -> None:
    _, patched_controller = patched_controller
    patched_controller.open_search_dialog()
    patched_controller.search_records(
        SearchCriteria(mode=FilterMode.BY_NAME_OR_GROUP, text_query=" ")
    )
    assert patched_controller._search_dialog.errors


def test_search_functions_noop_without_dialog(patched_controller) -> None:
    _, patched_controller = patched_controller
    patched_controller._search_dialog = None
    patched_controller.search_records(SearchCriteria(mode=FilterMode.BY_PENDING, pending_value=1))
    patched_controller.refresh_search_page()


def test_delete_dialog_remove_and_zero_results(patched_controller) -> None:
    _, patched_controller = patched_controller
    FakeDeleteDialog.auto_criteria = SearchCriteria(
        mode=FilterMode.BY_NAME_OR_GROUP,
        text_query="Unknown",
    )
    patched_controller.open_delete_dialog()
    assert "не найдены" in patched_controller._main_window.infos[-1]

    FakeDeleteDialog.auto_criteria = SearchCriteria(
        mode=FilterMode.BY_NAME_OR_GROUP,
        text_query="Ivan",
    )
    patched_controller.open_delete_dialog()
    assert "Удалено записей" in patched_controller._main_window.infos[-1]


def test_delete_error_is_shown_and_dialog_is_not_closed(patched_controller) -> None:
    _, patched_controller = patched_controller
    dialog = FakeDeleteDialog(
        patched_controller._main_window,
        on_delete=lambda _: None,
    )
    patched_controller._handle_delete_submit(
        dialog,
        SearchCriteria(mode=FilterMode.BY_NAME_OR_GROUP, text_query=" "),
    )
    assert patched_controller._main_window.errors
    assert not dialog.destroyed


def test_load_and_save_branches(patched_controller, monkeypatch, tmp_path) -> None:
    controller_module, patched_controller = patched_controller
    source_file = tmp_path / "source.xml"
    target_file = tmp_path / "target.xml"
    source_file.write_text("<students></students>", encoding="utf-8")

    loaded_records = [
        create_student_record(
            full_name="Loaded Student",
            course=1,
            group_name="P-11",
            total_works=5,
            completed_works=2,
            programming_language="Go",
        )
    ]

    monkeypatch.setattr(controller_module, "load_records_sax", lambda _: loaded_records)
    save_calls = []

    def fake_save(records, path: str) -> None:
        save_calls.append((records, path))

    monkeypatch.setattr(controller_module, "save_records_dom", fake_save)
    patched_controller._main_window.open_file = str(source_file)
    patched_controller.load_from_file()
    assert patched_controller._main_window.infos[-1] == "Файл успешно загружен."

    patched_controller._main_window.save_file = str(target_file)
    patched_controller.save_to_file()
    assert save_calls and save_calls[-1][1] == str(target_file)

    patched_controller._main_window.open_file = ""
    patched_controller.load_from_file()
    patched_controller._main_window.save_file = ""
    patched_controller.save_to_file()


def test_load_and_save_errors_are_shown(patched_controller, monkeypatch) -> None:
    controller_module, patched_controller = patched_controller
    patched_controller._main_window.open_file = "broken.xml"
    monkeypatch.setattr(
        controller_module,
        "load_records_sax",
        lambda _: (_ for _ in ()).throw(ValueError("bad xml")),
    )
    patched_controller.load_from_file()
    assert "Не удалось загрузить файл" in patched_controller._main_window.errors[-1]

    patched_controller._main_window.save_file = "out.xml"
    monkeypatch.setattr(
        controller_module,
        "save_records_dom",
        lambda *_: (_ for _ in ()).throw(OSError("disk full")),
    )
    patched_controller.save_to_file()
    assert "Не удалось сохранить файл" in patched_controller._main_window.errors[-1]


def test_sync_options_and_language_fallback(patched_controller) -> None:
    _, patched_controller = patched_controller
    patched_controller._search_dialog = FakeSearchDialog(
        patched_controller._main_window,
        on_search=lambda _: None,
        on_reset=lambda: None,
        on_navigate=lambda _: None,
        on_per_page_changed=lambda _: None,
    )
    patched_controller._sync_dialog_options()
    assert patched_controller._search_dialog.dropdowns is not None
    patched_controller._search_dialog.exists = False
    patched_controller._sync_dialog_options()
    patched_controller._service.replace_records([])
    assert patched_controller._get_language_options()


def test_delete_and_load_refresh_search_when_dialog_exists(
    patched_controller,
    monkeypatch,
) -> None:
    controller_module, patched_controller = patched_controller
    patched_controller._search_dialog = FakeSearchDialog(
        patched_controller._main_window,
        on_search=lambda _: None,
        on_reset=lambda: None,
        on_navigate=lambda _: None,
        on_per_page_changed=lambda _: None,
    )
    reset_calls = {"count": 0}

    def fake_reset() -> None:
        reset_calls["count"] += 1

    monkeypatch.setattr(patched_controller, "reset_search_results", fake_reset)
    dialog = FakeDeleteDialog(patched_controller._main_window, on_delete=lambda _: None)
    patched_controller._handle_delete_submit(
        dialog,
        SearchCriteria(mode=FilterMode.BY_NAME_OR_GROUP, text_query="Ivan"),
    )
    assert reset_calls["count"] == 1

    patched_controller._main_window.open_file = "from.xml"
    monkeypatch.setattr(controller_module, "load_records_sax", lambda _: [])
    patched_controller.load_from_file()
    assert reset_calls["count"] == 2


def test_navigate_to_page_all_actions(patched_controller) -> None:
    controller_module, _ = patched_controller
    assert controller_module._navigate_to_page("first", 3, 9) == 1
    assert controller_module._navigate_to_page("prev", 1, 9) == 1
    assert controller_module._navigate_to_page("next", 9, 9) == 9
    assert controller_module._navigate_to_page("last", 2, 9) == 9
    assert controller_module._navigate_to_page("unknown", 2, 9) == 2
