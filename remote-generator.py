template = """
        <Button
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="{0}"
            android:id="@+id/button_cd_{0}"
            android:layout_row="{1}"
            android:layout_column="{2}" />"""

for i in range(0, 12):
    row = i / 4
    column = i % 4
    print(template.format(i, row + 6, column))
