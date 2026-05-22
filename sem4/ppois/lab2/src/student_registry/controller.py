from student_registry.models import DomainError, SearchCriteria, create_student_record
from student_registry.services import StudentRegistryService, paginate
from student_registry.view import AddRecordDialog, DeleteDialog, MainWindow, SearchDialog
from student_registry.xml_storage import load_records_sax, save_records_dom


class AppController:
    def __init__(self, service: StudentRegistryService) -> None:
        self._service = service
        self._main_page = 1
        self._main_per_page = 10
        self._search_page = 1
        self._search_per_page = 10
        self._search_results: list = []
        self._search_dialog: SearchDialog | None = None
        self._main_window = MainWindow(
            on_add=self.open_add_dialog,
            on_search=self.open_search_dialog,
            on_delete=self.open_delete_dialog,
            on_load=self.load_from_file,
            on_save=self.save_to_file,
            on_navigate=self.navigate_main,
            on_per_page_changed=self.set_main_per_page,
        )

    def run(self) -> None:
        self.refresh_main_page()
        self._main_window.mainloop()

    def refresh_main_page(self) -> None:
        records = self._service.list_records()
        page_data = paginate(records, self._main_page, self._main_per_page)
        self._main_page = page_data.page
        self._main_window.set_page(page_data)

    def navigate_main(self, action: str) -> None:
        records = self._service.list_records()
        page_data = paginate(records, self._main_page, self._main_per_page)
        self._main_page = _navigate_to_page(action, page_data.page, page_data.total_pages)
        self.refresh_main_page()

    def set_main_per_page(self, per_page: int) -> None:
        self._main_per_page = per_page
        self._main_page = 1
        self.refresh_main_page()

    def open_add_dialog(self) -> None:
        dialog = AddRecordDialog(
            self._main_window,
            on_submit=lambda payload: self._handle_add_submit(dialog, payload),
            available_languages=self._get_language_options(),
        )
        dialog.wait_window()

    def _handle_add_submit(self, dialog: AddRecordDialog, payload: dict[str, str]) -> None:
        try:
            record = create_student_record(
                full_name=payload["full_name"],
                course=int(payload["course"]),
                group_name=payload["group_name"],
                total_works=int(payload["total_works"]),
                completed_works=int(payload["completed_works"]),
                programming_language=payload["programming_language"],
            )
            self._service.add_record(record)
        except (ValueError, DomainError) as error:
            self._main_window.show_error(str(error))
            return
        dialog.destroy()
        self.refresh_main_page()
        self._sync_dialog_options()

    def open_search_dialog(self) -> None:
        if self._search_dialog is not None and self._search_dialog.winfo_exists():
            self._search_dialog.focus()
            return
        self._search_results = self._service.list_records()
        self._search_page = 1
        self._search_dialog = SearchDialog(
            self._main_window,
            on_search=self.search_records,
            on_reset=self.reset_search_results,
            on_navigate=self.navigate_search,
            on_per_page_changed=self.set_search_per_page,
        )
        self._search_dialog.set_dropdown_values(
            self._service.get_languages(),
            self._service.get_completed_values(),
            self._service.get_total_values(),
        )
        self.refresh_search_page()

    def search_records(self, criteria: SearchCriteria) -> None:
        if self._search_dialog is None:
            return
        try:
            self._search_results = self._service.search(criteria)
        except DomainError as error:
            self._search_dialog.show_error(str(error))
            return
        self._search_page = 1
        self.refresh_search_page()

    def reset_search_results(self) -> None:
        self._search_results = self._service.list_records()
        self._search_page = 1
        self.refresh_search_page()

    def refresh_search_page(self) -> None:
        if self._search_dialog is None:
            return
        page_data = paginate(self._search_results, self._search_page, self._search_per_page)
        self._search_page = page_data.page
        self._search_dialog.set_page(page_data)

    def navigate_search(self, action: str) -> None:
        page_data = paginate(self._search_results, self._search_page, self._search_per_page)
        self._search_page = _navigate_to_page(action, page_data.page, page_data.total_pages)
        self.refresh_search_page()

    def set_search_per_page(self, per_page: int) -> None:
        self._search_per_page = per_page
        self._search_page = 1
        self.refresh_search_page()

    def open_delete_dialog(self) -> None:
        dialog = DeleteDialog(
            self._main_window,
            on_delete=lambda criteria: self._handle_delete_submit(dialog, criteria),
        )
        dialog.set_dropdown_values(
            self._service.get_languages(),
            self._service.get_completed_values(),
            self._service.get_total_values(),
        )
        dialog.wait_window()

    def _handle_delete_submit(self, dialog: DeleteDialog, criteria: SearchCriteria) -> None:
        try:
            removed_count = self._service.delete(criteria)
        except DomainError as error:
            self._main_window.show_error(str(error))
            return
        if removed_count == 0:
            self._main_window.show_info("Записи по указанному условию не найдены.")
        else:
            self._main_window.show_info(f"Удалено записей: {removed_count}.")
        dialog.destroy()
        self.refresh_main_page()
        if self._search_dialog is not None and self._search_dialog.winfo_exists():
            self.reset_search_results()
        self._sync_dialog_options()

    def load_from_file(self) -> None:
        file_path = self._main_window.ask_open_file()
        if not file_path:
            return
        try:
            records = load_records_sax(file_path)
            self._service.replace_records(records)
        except (DomainError, ValueError, OSError) as error:
            self._main_window.show_error(f"Не удалось загрузить файл: {error}")
            return
        self._main_page = 1
        self.refresh_main_page()
        if self._search_dialog is not None and self._search_dialog.winfo_exists():
            self.reset_search_results()
        self._sync_dialog_options()
        self._main_window.show_info("Файл успешно загружен.")

    def save_to_file(self) -> None:
        file_path = self._main_window.ask_save_file()
        if not file_path:
            return
        try:
            save_records_dom(self._service.list_records(), file_path)
        except OSError as error:
            self._main_window.show_error(f"Не удалось сохранить файл: {error}")
            return
        self._main_window.show_info("Файл успешно сохранен.")

    def _sync_dialog_options(self) -> None:
        if self._search_dialog is None or not self._search_dialog.winfo_exists():
            return
        self._search_dialog.set_dropdown_values(
            self._service.get_languages(),
            self._service.get_completed_values(),
            self._service.get_total_values(),
        )

    def _get_language_options(self) -> list[str]:
        existing = self._service.get_languages()
        if existing:
            return existing
        return ["Python", "Java", "C++", "JavaScript", "Go"]


def _navigate_to_page(action: str, current_page: int, total_pages: int) -> int:
    if action == "first":
        return 1
    if action == "prev":
        return max(1, current_page - 1)
    if action == "next":
        return min(total_pages, current_page + 1)
    if action == "last":
        return total_pages
    return current_page
