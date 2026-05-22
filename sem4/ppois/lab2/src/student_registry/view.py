import tkinter as tk
from tkinter import filedialog, messagebox, ttk
from typing import Callable

from student_registry.models import FilterMode, SearchCriteria, StudentRecord
from student_registry.services import Page


class RecordsTable(ttk.Frame):
    def __init__(self, master) -> None:
        super().__init__(master)
        columns = (
            "full_name",
            "course",
            "group_name",
            "total_works",
            "completed_works",
            "pending_works",
            "programming_language",
        )
        self.tree = ttk.Treeview(self, columns=columns, show="headings", height=12)
        self.tree.heading("full_name", text="ФИО студента")
        self.tree.heading("course", text="Курс")
        self.tree.heading("group_name", text="Группа")
        self.tree.heading("total_works", text="Общее число работ")
        self.tree.heading("completed_works", text="Выполненных работ")
        self.tree.heading("pending_works", text="Невыполненных работ")
        self.tree.heading("programming_language", text="Язык")
        self.tree.column("full_name", width=220, anchor=tk.W)
        self.tree.column("course", width=70, anchor=tk.CENTER)
        self.tree.column("group_name", width=120, anchor=tk.CENTER)
        self.tree.column("total_works", width=140, anchor=tk.CENTER)
        self.tree.column("completed_works", width=150, anchor=tk.CENTER)
        self.tree.column("pending_works", width=160, anchor=tk.CENTER)
        self.tree.column("programming_language", width=150, anchor=tk.CENTER)
        y_scroll = ttk.Scrollbar(self, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=y_scroll.set)
        self.tree.grid(row=0, column=0, sticky="nsew")
        y_scroll.grid(row=0, column=1, sticky="ns")
        self.rowconfigure(0, weight=1)
        self.columnconfigure(0, weight=1)

    def set_records(self, records: list[StudentRecord]) -> None:
        for item_id in self.tree.get_children():
            self.tree.delete(item_id)
        for record in records:
            self.tree.insert(
                "",
                tk.END,
                values=(
                    record.full_name,
                    record.course,
                    record.group_name,
                    record.total_works,
                    record.completed_works,
                    record.pending_works,
                    record.programming_language,
                ),
            )


class PaginationPanel(ttk.Frame):
    def __init__(
        self,
        master,
        on_navigate: Callable[[str], None],
        on_per_page_changed: Callable[[int], None],
    ) -> None:
        super().__init__(master)
        self._on_navigate = on_navigate
        self._on_per_page_changed = on_per_page_changed
        ttk.Button(self, text="<<", width=4, command=lambda: self._on_navigate("first")).pack(
            side=tk.LEFT,
            padx=2,
        )
        ttk.Button(self, text="<", width=4, command=lambda: self._on_navigate("prev")).pack(
            side=tk.LEFT,
            padx=2,
        )
        ttk.Button(self, text=">", width=4, command=lambda: self._on_navigate("next")).pack(
            side=tk.LEFT,
            padx=2,
        )
        ttk.Button(self, text=">>", width=4, command=lambda: self._on_navigate("last")).pack(
            side=tk.LEFT,
            padx=2,
        )
        ttk.Label(self, text="На странице:").pack(side=tk.LEFT, padx=(16, 4))
        self.per_page_combo = ttk.Combobox(
            self,
            state="readonly",
            width=5,
            values=("5", "10", "20", "50"),
        )
        self.per_page_combo.set("10")
        self.per_page_combo.pack(side=tk.LEFT)
        self.per_page_combo.bind("<<ComboboxSelected>>", self._handle_per_page_change)
        self.info_label = ttk.Label(self, text="Стр. 1/1 | Записей: 0")
        self.info_label.pack(side=tk.RIGHT, padx=4)

    def _handle_per_page_change(self, _) -> None:
        self._on_per_page_changed(int(self.per_page_combo.get()))

    def set_page_info(self, page_data: Page) -> None:
        current_count = len(page_data.items)
        self.info_label.config(
            text=(
                f"Стр. {page_data.page}/{page_data.total_pages} | "
                f"На странице: {current_count} | Всего: {page_data.total_items}"
            )
        )


class CriteriaFrame(ttk.LabelFrame):
    MODE_LABELS = {
        FilterMode.BY_NAME_OR_GROUP: "По ФИО или группе",
        FilterMode.BY_COURSE_OR_LANGUAGE: "По курсу или языку",
        FilterMode.BY_COMPLETED_OR_TOTAL: "По выполненным или общему числу",
        FilterMode.BY_PENDING: "По невыполненным",
    }

    def __init__(self, master) -> None:
        super().__init__(master, text="Условие")
        self._languages: list[str] = []
        self._completed_values: list[int] = []
        self._total_values: list[int] = []
        ttk.Label(self, text="Режим:").grid(row=0, column=0, sticky="w", padx=4, pady=4)
        self.mode_var = tk.StringVar(value=self.MODE_LABELS[FilterMode.BY_NAME_OR_GROUP])
        self.mode_combo = ttk.Combobox(
            self,
            state="readonly",
            textvariable=self.mode_var,
            values=tuple(self.MODE_LABELS.values()),
            width=38,
        )
        self.mode_combo.grid(row=0, column=1, sticky="ew", padx=4, pady=4)
        self.mode_combo.bind("<<ComboboxSelected>>", self._on_mode_changed)

        self.name_group_frame = ttk.Frame(self)
        ttk.Label(self.name_group_frame, text="ФИО или группа:").grid(
            row=0,
            column=0,
            sticky="w",
            padx=4,
            pady=4,
        )
        self.query_entry = ttk.Entry(self.name_group_frame, width=36)
        self.query_entry.grid(row=0, column=1, sticky="ew", padx=4, pady=4)

        self.course_language_frame = ttk.Frame(self)
        self.course_or_language_var = tk.StringVar(value="course")
        ttk.Radiobutton(
            self.course_language_frame,
            text="Курс",
            variable=self.course_or_language_var,
            value="course",
        ).grid(row=0, column=0, sticky="w", padx=4, pady=4)
        ttk.Radiobutton(
            self.course_language_frame,
            text="Язык",
            variable=self.course_or_language_var,
            value="language",
        ).grid(row=0, column=1, sticky="w", padx=4, pady=4)
        self.course_spinbox = ttk.Spinbox(self.course_language_frame, from_=1, to=6, width=6)
        self.course_spinbox.set("1")
        self.course_spinbox.grid(row=0, column=2, sticky="w", padx=4, pady=4)
        self.language_combo = ttk.Combobox(self.course_language_frame, width=18)
        self.language_combo.grid(row=0, column=3, sticky="w", padx=4, pady=4)

        self.completed_total_frame = ttk.Frame(self)
        self.completed_or_total_var = tk.StringVar(value="completed")
        ttk.Radiobutton(
            self.completed_total_frame,
            text="Выполненные",
            variable=self.completed_or_total_var,
            value="completed",
        ).grid(row=0, column=0, sticky="w", padx=4, pady=4)
        ttk.Radiobutton(
            self.completed_total_frame,
            text="Общее число",
            variable=self.completed_or_total_var,
            value="total",
        ).grid(row=0, column=1, sticky="w", padx=4, pady=4)
        self.completed_combo = ttk.Combobox(self.completed_total_frame, width=10)
        self.completed_combo.grid(row=0, column=2, sticky="w", padx=4, pady=4)
        self.total_combo = ttk.Combobox(self.completed_total_frame, width=10)
        self.total_combo.grid(row=0, column=3, sticky="w", padx=4, pady=4)

        self.pending_frame = ttk.Frame(self)
        ttk.Label(self.pending_frame, text="Невыполненные работы:").grid(
            row=0,
            column=0,
            sticky="w",
            padx=4,
            pady=4,
        )
        self.pending_spinbox = ttk.Spinbox(self.pending_frame, from_=0, to=999, width=8)
        self.pending_spinbox.set("0")
        self.pending_spinbox.grid(row=0, column=1, sticky="w", padx=4, pady=4)

        self.columnconfigure(1, weight=1)
        self._on_mode_changed(None)

    def set_dropdown_values(
        self,
        languages: list[str],
        completed_values: list[int],
        total_values: list[int],
    ) -> None:
        self._languages = languages
        self._completed_values = completed_values
        self._total_values = total_values
        self.language_combo["values"] = tuple(languages)
        if languages and not self.language_combo.get():
            self.language_combo.set(languages[0])
        completed_strings = [str(value) for value in completed_values]
        self.completed_combo["values"] = tuple(completed_strings)
        if completed_strings and not self.completed_combo.get():
            self.completed_combo.set(completed_strings[0])
        total_strings = [str(value) for value in total_values]
        self.total_combo["values"] = tuple(total_strings)
        if total_strings and not self.total_combo.get():
            self.total_combo.set(total_strings[0])

    def _on_mode_changed(self, _) -> None:
        self.name_group_frame.grid_forget()
        self.course_language_frame.grid_forget()
        self.completed_total_frame.grid_forget()
        self.pending_frame.grid_forget()
        mode = self.get_selected_mode()
        if mode is FilterMode.BY_NAME_OR_GROUP:
            self.name_group_frame.grid(row=1, column=0, columnspan=2, sticky="ew")
        elif mode is FilterMode.BY_COURSE_OR_LANGUAGE:
            self.course_language_frame.grid(row=1, column=0, columnspan=2, sticky="ew")
        elif mode is FilterMode.BY_COMPLETED_OR_TOTAL:
            self.completed_total_frame.grid(row=1, column=0, columnspan=2, sticky="ew")
        else:
            self.pending_frame.grid(row=1, column=0, columnspan=2, sticky="ew")

    def get_selected_mode(self) -> FilterMode:
        for mode, label in self.MODE_LABELS.items():
            if self.mode_var.get() == label:
                return mode
        return FilterMode.BY_NAME_OR_GROUP

    def get_criteria(self) -> SearchCriteria:
        mode = self.get_selected_mode()
        if mode is FilterMode.BY_NAME_OR_GROUP:
            return SearchCriteria(mode=mode, text_query=self.query_entry.get())
        if mode is FilterMode.BY_COURSE_OR_LANGUAGE:
            use_course = self.course_or_language_var.get() == "course"
            return SearchCriteria(
                mode=mode,
                use_course=use_course,
                course_value=int(self.course_spinbox.get()) if use_course else None,
                language_value=self.language_combo.get() if not use_course else None,
            )
        if mode is FilterMode.BY_COMPLETED_OR_TOTAL:
            use_completed = self.completed_or_total_var.get() == "completed"
            completed_value = int(self.completed_combo.get()) if use_completed else None
            total_value = int(self.total_combo.get()) if not use_completed else None
            return SearchCriteria(
                mode=mode,
                use_completed=use_completed,
                completed_value=completed_value,
                total_value=total_value,
            )
        return SearchCriteria(mode=mode, pending_value=int(self.pending_spinbox.get()))


class AddRecordDialog(tk.Toplevel):
    def __init__(
        self,
        parent,
        on_submit: Callable[[dict[str, str]], None],
        available_languages: list[str],
    ) -> None:
        super().__init__(parent)
        self.title("Добавление записи")
        self.transient(parent)
        self.grab_set()
        self._on_submit = on_submit

        frame = ttk.Frame(self, padding=12)
        frame.pack(fill=tk.BOTH, expand=True)
        ttk.Label(frame, text="ФИО:").grid(row=0, column=0, sticky="w", padx=4, pady=4)
        self.name_entry = ttk.Entry(frame, width=36)
        self.name_entry.grid(row=0, column=1, sticky="ew", padx=4, pady=4)
        ttk.Label(frame, text="Курс:").grid(row=1, column=0, sticky="w", padx=4, pady=4)
        self.course_spinbox = ttk.Spinbox(frame, from_=1, to=6, width=6)
        self.course_spinbox.set("1")
        self.course_spinbox.grid(row=1, column=1, sticky="w", padx=4, pady=4)
        ttk.Label(frame, text="Группа:").grid(row=2, column=0, sticky="w", padx=4, pady=4)
        self.group_entry = ttk.Entry(frame, width=36)
        self.group_entry.grid(row=2, column=1, sticky="ew", padx=4, pady=4)
        ttk.Label(frame, text="Общее число работ:").grid(row=3, column=0, sticky="w", padx=4, pady=4)
        self.total_spinbox = ttk.Spinbox(frame, from_=0, to=999, width=8)
        self.total_spinbox.set("10")
        self.total_spinbox.grid(row=3, column=1, sticky="w", padx=4, pady=4)
        ttk.Label(frame, text="Выполненные работы:").grid(row=4, column=0, sticky="w", padx=4, pady=4)
        self.completed_spinbox = ttk.Spinbox(frame, from_=0, to=999, width=8)
        self.completed_spinbox.set("0")
        self.completed_spinbox.grid(row=4, column=1, sticky="w", padx=4, pady=4)
        ttk.Label(frame, text="Язык:").grid(row=5, column=0, sticky="w", padx=4, pady=4)
        self.language_combo = ttk.Combobox(
            frame,
            width=20,
            values=tuple(available_languages),
        )
        if available_languages:
            self.language_combo.set(available_languages[0])
        self.language_combo.grid(row=5, column=1, sticky="w", padx=4, pady=4)

        buttons = ttk.Frame(frame)
        buttons.grid(row=6, column=0, columnspan=2, pady=(10, 0), sticky="e")
        ttk.Button(buttons, text="Добавить", command=self._submit).pack(side=tk.LEFT, padx=4)
        ttk.Button(buttons, text="Отмена", command=self.destroy).pack(side=tk.LEFT, padx=4)
        frame.columnconfigure(1, weight=1)

    def _submit(self) -> None:
        payload = {
            "full_name": self.name_entry.get(),
            "course": self.course_spinbox.get(),
            "group_name": self.group_entry.get(),
            "total_works": self.total_spinbox.get(),
            "completed_works": self.completed_spinbox.get(),
            "programming_language": self.language_combo.get(),
        }
        self._on_submit(payload)


class SearchDialog(tk.Toplevel):
    def __init__(
        self,
        parent,
        on_search: Callable[[SearchCriteria], None],
        on_reset: Callable[[], None],
        on_navigate: Callable[[str], None],
        on_per_page_changed: Callable[[int], None],
    ) -> None:
        super().__init__(parent)
        self.title("Поиск записей")
        self.geometry("1060x560")
        self.transient(parent)
        self.grab_set()
        self._on_search = on_search
        self._on_reset = on_reset
        frame = ttk.Frame(self, padding=8)
        frame.pack(fill=tk.BOTH, expand=True)
        self.criteria_frame = CriteriaFrame(frame)
        self.criteria_frame.pack(fill=tk.X, pady=(0, 8))
        button_row = ttk.Frame(frame)
        button_row.pack(fill=tk.X, pady=(0, 6))
        ttk.Button(button_row, text="Найти", command=self._handle_search).pack(
            side=tk.LEFT,
            padx=4,
        )
        ttk.Button(button_row, text="Сброс", command=self._on_reset).pack(side=tk.LEFT, padx=4)
        self.table = RecordsTable(frame)
        self.table.pack(fill=tk.BOTH, expand=True)
        self.pagination_panel = PaginationPanel(
            frame,
            on_navigate=on_navigate,
            on_per_page_changed=on_per_page_changed,
        )
        self.pagination_panel.pack(fill=tk.X, pady=(6, 0))

    def set_dropdown_values(
        self,
        languages: list[str],
        completed_values: list[int],
        total_values: list[int],
    ) -> None:
        self.criteria_frame.set_dropdown_values(languages, completed_values, total_values)

    def _handle_search(self) -> None:
        self._on_search(self.criteria_frame.get_criteria())

    def set_page(self, page_data: Page) -> None:
        self.table.set_records(page_data.items)
        self.pagination_panel.set_page_info(page_data)

    def show_error(self, message: str) -> None:
        messagebox.showerror("Ошибка", message, parent=self)


class DeleteDialog(tk.Toplevel):
    def __init__(
        self,
        parent,
        on_delete: Callable[[SearchCriteria], None],
    ) -> None:
        super().__init__(parent)
        self.title("Удаление записей")
        self.transient(parent)
        self.grab_set()
        self._on_delete = on_delete
        frame = ttk.Frame(self, padding=8)
        frame.pack(fill=tk.BOTH, expand=True)
        self.criteria_frame = CriteriaFrame(frame)
        self.criteria_frame.pack(fill=tk.X, pady=(0, 8))
        button_row = ttk.Frame(frame)
        button_row.pack(fill=tk.X)
        ttk.Button(button_row, text="Удалить", command=self._handle_delete).pack(
            side=tk.LEFT,
            padx=4,
        )
        ttk.Button(button_row, text="Отмена", command=self.destroy).pack(side=tk.LEFT, padx=4)

    def set_dropdown_values(
        self,
        languages: list[str],
        completed_values: list[int],
        total_values: list[int],
    ) -> None:
        self.criteria_frame.set_dropdown_values(languages, completed_values, total_values)

    def _handle_delete(self) -> None:
        self._on_delete(self.criteria_frame.get_criteria())


class MainWindow(tk.Tk):
    def __init__(
        self,
        on_add: Callable[[], None],
        on_search: Callable[[], None],
        on_delete: Callable[[], None],
        on_load: Callable[[], None],
        on_save: Callable[[], None],
        on_navigate: Callable[[str], None],
        on_per_page_changed: Callable[[int], None],
    ) -> None:
        super().__init__()
        self.title("Учёт учебных работ студентов")
        self.geometry("1150x640")
        self._create_menu(on_add, on_search, on_delete, on_load, on_save)
        root = ttk.Frame(self, padding=8)
        root.pack(fill=tk.BOTH, expand=True)
        self._create_toolbar(root, on_add, on_search, on_delete, on_load, on_save)
        self.table = RecordsTable(root)
        self.table.pack(fill=tk.BOTH, expand=True, pady=(8, 0))
        self.pagination_panel = PaginationPanel(
            root,
            on_navigate=on_navigate,
            on_per_page_changed=on_per_page_changed,
        )
        self.pagination_panel.pack(fill=tk.X, pady=(8, 0))

    def _create_menu(
        self,
        on_add: Callable[[], None],
        on_search: Callable[[], None],
        on_delete: Callable[[], None],
        on_load: Callable[[], None],
        on_save: Callable[[], None],
    ) -> None:
        menu_bar = tk.Menu(self)
        file_menu = tk.Menu(menu_bar, tearoff=0)
        file_menu.add_command(label="Загрузить XML", command=on_load)
        file_menu.add_command(label="Сохранить XML", command=on_save)
        file_menu.add_separator()
        file_menu.add_command(label="Выход", command=self.destroy)
        actions_menu = tk.Menu(menu_bar, tearoff=0)
        actions_menu.add_command(label="Добавить запись", command=on_add)
        actions_menu.add_command(label="Поиск", command=on_search)
        actions_menu.add_command(label="Удаление", command=on_delete)
        menu_bar.add_cascade(label="Файл", menu=file_menu)
        menu_bar.add_cascade(label="Действия", menu=actions_menu)
        self.config(menu=menu_bar)

    def _create_toolbar(
        self,
        parent,
        on_add: Callable[[], None],
        on_search: Callable[[], None],
        on_delete: Callable[[], None],
        on_load: Callable[[], None],
        on_save: Callable[[], None],
    ) -> None:
        toolbar = ttk.Frame(parent)
        toolbar.pack(fill=tk.X)
        ttk.Button(toolbar, text="Добавить", command=on_add).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Поиск", command=on_search).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Удалить", command=on_delete).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Загрузить", command=on_load).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Сохранить", command=on_save).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Выход", command=self.destroy).pack(side=tk.LEFT, padx=2)

    def set_page(self, page_data: Page) -> None:
        self.table.set_records(page_data.items)
        self.pagination_panel.set_page_info(page_data)

    def ask_open_file(self) -> str:
        return filedialog.askopenfilename(
            parent=self,
            title="Загрузка XML",
            filetypes=[("XML files", "*.xml"), ("All files", "*.*")],
        )

    def ask_save_file(self) -> str:
        return filedialog.asksaveasfilename(
            parent=self,
            title="Сохранение XML",
            defaultextension=".xml",
            filetypes=[("XML files", "*.xml"), ("All files", "*.*")],
        )

    def show_info(self, message: str) -> None:
        messagebox.showinfo("Информация", message, parent=self)

    def show_error(self, message: str) -> None:
        messagebox.showerror("Ошибка", message, parent=self)
