# Copyright (C) 2013-2017 Mattia Basaglia
#
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/color_preview.cpp \
    $$PWD/color_wheel.cpp \
    $$PWD/gradient_slider.cpp \
    $$PWD/hue_slider.cpp \
    $$PWD/color_dialog.cpp \
    $$PWD/color_selector.cpp \
    $$PWD/color_delegate.cpp \
    $$PWD/bound_color_selector.cpp \
    $$PWD/color_palette.cpp \
    $$PWD/color_palette_model.cpp \
    $$PWD/color_palette_widget.cpp \
    $$PWD/swatch.cpp \
    $$PWD/color_utils.cpp \
    $$PWD/color_2d_slider.cpp \
    $$PWD/color_line_edit.cpp \
    $$PWD/color_names.cpp \
    $$PWD/gradient_editor.cpp

HEADERS += \
    $$PWD/color_wheel.hpp \
    $$PWD/color_preview.hpp \
    $$PWD/gradient_slider.hpp \
    $$PWD/hue_slider.hpp \
    $$PWD/color_dialog.hpp \
    $$PWD/color_selector.hpp \
    $$PWD/color_delegate.hpp \
    $$PWD/bound_color_selector.hpp \
    $$PWD/color_palette.hpp \
    $$PWD/color_palette_model.hpp \
    $$PWD/color_palette_widget.hpp \
    $$PWD/swatch.hpp \
    $$PWD/color_utils.hpp \
    $$PWD/color_2d_slider.hpp \
    $$PWD/color_line_edit.hpp \
    $$PWD/color_names.hpp \
    $$PWD/gradient_editor.hpp

FORMS += \
    $$PWD/color_dialog.ui \
    $$PWD/color_palette_widget.ui

RESOURCES += \
    $$PWD/resources/QtColorWidgets/color_widgets.qrc


