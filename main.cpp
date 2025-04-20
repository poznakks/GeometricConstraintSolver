// ReSharper disable CppDFAMemoryLeak
#include <wx/wx.h>
#include "objects/point.h"
#include "objects/line.h"
#include <cmath>

#include "types.h"
#include "constraints/constraint.h"
#include "constraints/l2l_distance_constraint.h"
#include "constraints/l2l_parallel_constraint.h"
#include "constraints/p2p_horizontal_constraint.h"
#include "constraints/p2l_distance_constraint.h"
#include "constraints/p2p_vertical_constraint.h"
#include "constraints/p2p_distance_constraint.h"

enum ID {
    ID_Point = 1,
    ID_Line,
    ID_Constraint
};

enum class ConstraintType {
    P2PHorizontal,
    P2PVertical,
    P2PDistance,
    P2LDistance,
    L2LParallel,
    L2LDistance
};

inline wxString ConstraintTypeToString(const ConstraintType type) {
    switch (type) {
        case ConstraintType::P2PHorizontal: return "Points horizontal";
        case ConstraintType::P2PVertical: return "Points vertical";
        case ConstraintType::P2PDistance: return "Point to point distance";
        case ConstraintType::P2LDistance: return "Point to line distance";
        case ConstraintType::L2LParallel: return "Lines parallel";
        case ConstraintType::L2LDistance: return "Line to Line Distance";
        default: return "Unknown";
    }
}

inline ConstraintType ConstraintTypeFromString(const wxString& str) {
    if (str == "Points horizontal") return ConstraintType::P2PHorizontal;
    if (str == "Points vertical") return ConstraintType::P2PVertical;
    if (str == "Point to point distance") return ConstraintType::P2PDistance;
    if (str == "Point to line distance") return ConstraintType::P2LDistance;
    if (str == "Lines parallel") return ConstraintType::L2LParallel;
    if (str == "Line to Line Distance") return ConstraintType::L2LDistance;
    throw std::invalid_argument("Unknown constraint string: " + std::string(str.mb_str()));
}

class L2LDistanceConstraintDialog final : public wxDialog {
public:
    L2LDistanceConstraintDialog(wxWindow* parent, const VectorLineSharedPtr& lines)
        : wxDialog(parent, wxID_ANY, "Add Line-to-Line Distance Constraint") {

        auto* sizer = new wxBoxSizer(wxVERTICAL);

        // Первая прямая
        sizer->Add(new wxStaticText(this, wxID_ANY, "Select first line:"), 0, wxALL, 5);
        line1Choice = new wxChoice(this, wxID_ANY);
        sizer->Add(line1Choice, 0, wxALL | wxEXPAND, 5);

        // Вторая прямая
        sizer->Add(new wxStaticText(this, wxID_ANY, "Select second line:"), 0, wxALL, 5);
        line2Choice = new wxChoice(this, wxID_ANY);
        sizer->Add(line2Choice, 0, wxALL | wxEXPAND, 5);

        // Заполнение списка прямых
        for (size_t i = 0; i < lines.size(); ++i) {
            wxString label = wxString::Format("Line %d (%.1f, %.1f dir %.2f, %.2f)",
                                              static_cast<int>(i),
                                              lines[i]->point.x, lines[i]->point.y,
                                              lines[i]->direction.x, lines[i]->direction.y);
            line1Choice->Append(label);
            line2Choice->Append(label);
        }

        // Поле ввода расстояния
        sizer->Add(new wxStaticText(this, wxID_ANY, "Target distance:"), 0, wxALL, 5);
        distanceCtrl = new wxTextCtrl(this, wxID_ANY);
        sizer->Add(distanceCtrl, 0, wxALL | wxEXPAND, 5);

        // Кнопки OK / Cancel
        auto* buttonSizer = new wxStdDialogButtonSizer();
        buttonSizer->AddButton(new wxButton(this, wxID_OK));
        buttonSizer->AddButton(new wxButton(this, wxID_CANCEL));
        buttonSizer->Realize();

        sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        SetSizerAndFit(sizer);
    }

    int GetFirstIndex() const { return line1Choice->GetSelection(); }
    int GetSecondIndex() const { return line2Choice->GetSelection(); }

    double GetDistance() const {
        double value = 0;
        distanceCtrl->GetValue().ToDouble(&value);
        return value;
    }

private:
    wxChoice* line1Choice;
    wxChoice* line2Choice;
    wxTextCtrl* distanceCtrl;
};

class L2LParallelConstraintDialog final : public wxDialog {
public:
    L2LParallelConstraintDialog(wxWindow* parent, const VectorLineSharedPtr& lines)
        : wxDialog(parent, wxID_ANY, "Add Parallel Lines Constraint") {

        auto* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select first line:"), 0, wxALL, 5);
        choice1 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice1, 0, wxALL | wxEXPAND, 5);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select second line:"), 0, wxALL, 5);
        choice2 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice2, 0, wxALL | wxEXPAND, 5);

        for (size_t i = 0; i < lines.size(); ++i) {
            wxString label = wxString::Format("Line %d (%.1f, %.1f dir %.2f, %.2f)",
                                              static_cast<int>(i),
                                              lines[i]->point.x, lines[i]->point.y,
                                              lines[i]->direction.x, lines[i]->direction.y);
            choice1->Append(label);
            choice2->Append(label);
        }

        auto* buttonSizer = new wxStdDialogButtonSizer();
        buttonSizer->AddButton(new wxButton(this, wxID_OK));
        buttonSizer->AddButton(new wxButton(this, wxID_CANCEL));
        buttonSizer->Realize();

        sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        SetSizerAndFit(sizer);
    }

    int GetFirstIndex() const { return choice1->GetSelection(); }
    int GetSecondIndex() const { return choice2->GetSelection(); }

private:
    wxChoice* choice1;
    wxChoice* choice2;
};

class P2LDistanceConstraintDialog final : public wxDialog {
public:
    P2LDistanceConstraintDialog(wxWindow* parent, const VectorPointSharedPtr& points, const VectorLineSharedPtr& lines)
        : wxDialog(parent, wxID_ANY, "Add Point-to-Line Constraint") {

        auto* sizer = new wxBoxSizer(wxVERTICAL);

        // Выбор точки
        sizer->Add(new wxStaticText(this, wxID_ANY, "Select point:"), 0, wxALL, 5);
        pointChoice = new wxChoice(this, wxID_ANY);
        sizer->Add(pointChoice, 0, wxALL | wxEXPAND, 5);

        // Заполнение списка точек
        for (size_t i = 0; i < points.size(); ++i) {
            wxString label = wxString::Format("Point %d (%.1f, %.1f)", static_cast<int>(i), points[i]->x, points[i]->y);
            pointChoice->Append(label);
        }

        // Выбор прямой
        sizer->Add(new wxStaticText(this, wxID_ANY, "Select line:"), 0, wxALL, 5);
        lineChoice = new wxChoice(this, wxID_ANY);
        sizer->Add(lineChoice, 0, wxALL | wxEXPAND, 5);

        // Заполнение списка прямых
        for (size_t i = 0; i < lines.size(); ++i) {
            wxString label = wxString::Format("Line %d (%.1f, %.1f dir %.2f, %.2f)",
                                              static_cast<int>(i),
                                              lines[i]->point.x, lines[i]->point.y,
                                              lines[i]->direction.x, lines[i]->direction.y);
            lineChoice->Append(label);
        }

        // Поле ввода расстояния
        sizer->Add(new wxStaticText(this, wxID_ANY, "Target distance:"), 0, wxALL, 5);
        distanceCtrl = new wxTextCtrl(this, wxID_ANY);
        sizer->Add(distanceCtrl, 0, wxALL | wxEXPAND, 5);

        // Кнопки OK / Cancel
        auto* buttonSizer = new wxStdDialogButtonSizer();
        buttonSizer->AddButton(new wxButton(this, wxID_OK));
        buttonSizer->AddButton(new wxButton(this, wxID_CANCEL));
        buttonSizer->Realize();

        sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        SetSizerAndFit(sizer);
    }

    int GetPointIndex() const { return pointChoice->GetSelection(); }
    int GetLineIndex() const { return lineChoice->GetSelection(); }

    double GetDistance() const {
        double value = 0;
        distanceCtrl->GetValue().ToDouble(&value);
        return value;
    }

private:
    wxChoice* pointChoice;
    wxChoice* lineChoice;
    wxTextCtrl* distanceCtrl;
};

class P2PDistanceConstraintDialog final : public wxDialog {
public:
    P2PDistanceConstraintDialog(wxWindow* parent, const VectorPointSharedPtr& points)
        : wxDialog(parent, wxID_ANY, "Add Distance Constraint") {

        auto* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select first point:"), 0, wxALL, 5);
        choice1 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice1, 0, wxALL | wxEXPAND, 5);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select second point:"), 0, wxALL, 5);
        choice2 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice2, 0, wxALL | wxEXPAND, 5);

        for (size_t i = 0; i < points.size(); ++i) {
            wxString label = wxString::Format("Point %d (%.1f, %.1f)", static_cast<int>(i), points[i]->x, points[i]->y);
            choice1->Append(label);
            choice2->Append(label);
        }

        sizer->Add(new wxStaticText(this, wxID_ANY, "Target distance:"), 0, wxALL, 5);
        distanceCtrl = new wxTextCtrl(this, wxID_ANY);
        sizer->Add(distanceCtrl, 0, wxALL | wxEXPAND, 5);

        auto* buttonSizer = new wxStdDialogButtonSizer();
        buttonSizer->AddButton(new wxButton(this, wxID_OK));
        buttonSizer->AddButton(new wxButton(this, wxID_CANCEL));
        buttonSizer->Realize();

        sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        SetSizerAndFit(sizer);
    }

    int GetFirstIndex() const { return choice1->GetSelection(); }
    int GetSecondIndex() const { return choice2->GetSelection(); }

    double GetDistance() const {
        double value = 0;
        distanceCtrl->GetValue().ToDouble(&value);
        return value;
    }

private:
    wxChoice* choice1;
    wxChoice* choice2;
    wxTextCtrl* distanceCtrl;
};

class P2PConstraintDialog final : public wxDialog {
public:
    P2PConstraintDialog(wxWindow* parent, const VectorPointSharedPtr& points)
        : wxDialog(parent, wxID_ANY, "Add Constraint", wxDefaultPosition, wxDefaultSize) {

        auto* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select first point:"), 0, wxALL, 5);
        choice1 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice1, 0, wxALL | wxEXPAND, 5);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select second point:"), 0, wxALL, 5);
        choice2 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice2, 0, wxALL | wxEXPAND, 5);

        for (size_t i = 0; i < points.size(); ++i) {
            wxString label = wxString::Format("Point %d (%.1f, %.1f)", static_cast<int>(i), points[i]->x, points[i]->y);
            choice1->Append(label);
            choice2->Append(label);
        }

        auto* buttonSizer = new wxStdDialogButtonSizer();
        buttonSizer->AddButton(new wxButton(this, wxID_OK));
        buttonSizer->AddButton(new wxButton(this, wxID_CANCEL));
        buttonSizer->Realize();

        sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        SetSizerAndFit(sizer);
    }

    int GetFirstIndex() const {
        return choice1->GetSelection();
    }

    int GetSecondIndex() const {
        return choice2->GetSelection();
    }

private:
    wxChoice* choice1;
    wxChoice* choice2;
};

// Dialog to input coordinates for a point or line
class CoordinateDialog final : public wxDialog {
public:
    CoordinateDialog(wxWindow* parent, const wxString& title)
        : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150)) {
        auto* sizer = new wxBoxSizer(wxVERTICAL);

        // Add text fields for x and y coordinates
        auto* xLabel = new wxStaticText(this, wxID_ANY, "X:");
        xInput = new wxTextCtrl(this, wxID_ANY);
        auto* yLabel = new wxStaticText(this, wxID_ANY, "Y:");
        yInput = new wxTextCtrl(this, wxID_ANY);

        // Arrange the input fields in a grid
        auto* grid = new wxGridSizer(2, 2, 5, 5);
        grid->Add(xLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
        grid->Add(xInput, 0, wxEXPAND);
        grid->Add(yLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
        grid->Add(yInput, 0, wxEXPAND);

        sizer->Add(grid, 1, wxALL | wxEXPAND, 10);

        // Add OK and Cancel buttons
        sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, 10);

        SetSizer(sizer);
    }

    // Get the values from the dialog
    double GetX() const {
        double x;
        xInput->GetValue().ToDouble(&x);
        return x;
    }

    double GetY() const {
        double y;
        yInput->GetValue().ToDouble(&y);
        return y;
    }

private:
    wxTextCtrl* xInput;
    wxTextCtrl* yInput;
};

class MyCanvas final : public wxPanel {
public:
    explicit MyCanvas(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
        Bind(wxEVT_PAINT, &MyCanvas::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &MyCanvas::OnLeftClick, this);
        Bind(wxEVT_MOTION, &MyCanvas::OnMouseMove, this);
        Bind(wxEVT_LEFT_UP, &MyCanvas::OnLeftUp, this);
        Bind(wxEVT_LEFT_DCLICK, &MyCanvas::OnLeftDoubleClick, this);
    }

    void AddPoint(const PointSharedPtr& gcs_point) {
        points.push_back(gcs_point);
        Refresh();  // Redraw the canvas
    }

    void AddLine(const LineSharedPtr& gcs_line) {
        lines.push_back(gcs_line);
        Refresh();  // Redraw the canvas
    }

    void AddConstraint(ConstraintUniquePtr constraint) {
        constraints.push_back(std::move(constraint));
        ApplyConstraints(); // сразу применить при добавлении
    }

    VectorPointSharedPtr GetPoints() const { return points; }
    VectorLineSharedPtr GetLines() const { return lines; }

private:
    VectorPointSharedPtr points;  // Points from GCS
    VectorLineSharedPtr lines;    // Lines from GCS
    VectorConstraintUniquePtr constraints;
    bool isDragging = false;
    bool isRotating = false;
    unsigned long rotatingLineIndex = -1;
    unsigned long draggingPointIndex = -1;  // To track the point being dragged
    unsigned long draggingLineIndex = -1;   // To track the line being dragged
    wxPoint lastMousePos;

    void ApplyConstraints() const {
        for (const auto& constraint : constraints) {
            constraint->apply();  // Применить ограничение
        }
    }

    void OnPaint(wxPaintEvent&) {
        wxPaintDC dc(this);
        dc.Clear();
        dc.SetPen(*wxBLACK_PEN);

        // Draw points
        for (const auto& point : points) {
            dc.DrawCircle(wxPoint(static_cast<int>(point->x), static_cast<int>(point->y)), 5);  // GCS points mapped to canvas points
        }

        dc.SetPen(*wxBLUE_PEN);
        // Draw lines
        for (const auto& line : lines) {
            constexpr int length = 1000;
            wxPoint p1(static_cast<int>(line->point.x - line->direction.x * length), static_cast<int>(line->point.y - line->direction.y * length));
            wxPoint p2(static_cast<int>(line->point.x + line->direction.x * length), static_cast<int>(line->point.y + line->direction.y * length));
            dc.DrawLine(p1, p2);
        }
    }

    // Helper function to check if a click is near a line
    static bool IsNearLine(const LineSharedPtr& line, const wxPoint& pos) {
        constexpr double threshold = 5.0;
        const wxPoint p1(static_cast<int>(line->point.x), static_cast<int>(line->point.y));
        const wxPoint p2(static_cast<int>(line->point.x + line->direction.x * 10), static_cast<int>(line->point.y + line->direction.y * 10));

        // Calculate the distance from the click to the line using a point-line distance formula
        const double distance = std::abs((p2.y - p1.y) * pos.x - (p2.x - p1.x) * pos.y + p2.x * p1.y - p2.y * p1.x) /
                                std::sqrt(std::pow(p2.y - p1.y, 2) + std::pow(p2.x - p1.x, 2));

        return distance < threshold;
    }

    void OnLeftClick(const wxMouseEvent& event) {
        const wxPoint pos = event.GetPosition();
        isDragging = false;
        draggingPointIndex = -1;
        draggingLineIndex = -1;

        // Check if a point is clicked
        for (size_t i = 0; i < points.size(); ++i) {
            if (abs(points[i]->x - pos.x) < 5 && abs(points[i]->y - pos.y) < 5) {
                isDragging = true;
                draggingPointIndex = i;  // Start dragging this point
                lastMousePos = pos;
                return;
            }
        }

        // Check if a line is clicked
        for (size_t i = 0; i < lines.size(); ++i) {
            if (IsNearLine(lines[i], pos)) {
                isDragging = true;
                draggingLineIndex = i;  // Start dragging this line
                lastMousePos = pos;
                return;
            }
        }
    }

    void OnLeftDoubleClick(const wxMouseEvent& event) {
        const wxPoint pos = event.GetPosition();

        for (size_t i = 0; i < lines.size(); ++i) {
            if (IsNearLine(lines[i], pos)) {
                isRotating = true;
                rotatingLineIndex = i;
                lastMousePos = pos;
                CaptureMouse();  // Зафиксировать мышь
                return;
            }
        }
    }

    void OnMouseMove(const wxMouseEvent& event) {
        const wxPoint pos = event.GetPosition();
        if (isDragging) {
            const int dx = pos.x - lastMousePos.x;
            const int dy = pos.y - lastMousePos.y;

            // If dragging a point
            if (draggingPointIndex != -1) {
                points[draggingPointIndex]->x += dx;
                points[draggingPointIndex]->y += dy;
            }

            // If dragging a line
            if (draggingLineIndex != -1) {
                lines[draggingLineIndex]->point.x += dx;
                lines[draggingLineIndex]->point.y += dy;
            }
        }

        if (isRotating && rotatingLineIndex != -1) {
            const auto line = lines[rotatingLineIndex];
            const Point origin(line->point.x, line->point.y);

            double dx1 = lastMousePos.x - origin.x;
            double dy1 = lastMousePos.y - origin.y;
            double dx2 = pos.x - origin.x;
            double dy2 = pos.y - origin.y;

            double angle1 = std::atan2(dy1, dx1);
            double angle2 = std::atan2(dy2, dx2);
            double deltaAngle = angle2 - angle1;

            Point& dir = line->direction;

            // Поворот
            double cosA = std::cos(deltaAngle);
            double sinA = std::sin(deltaAngle);

            double newX = dir.x * cosA - dir.y * sinA;
            double newY = dir.x * sinA + dir.y * cosA;

            dir.x = newX;
            dir.y = newY;
        }
        lastMousePos = pos;
        ApplyConstraints();
        Refresh();  // Redraw canvas with updated point or line position
    }

    void OnLeftUp(wxMouseEvent&) {
        if (isDragging) {
            isDragging = false;
            draggingPointIndex = -1;
            draggingLineIndex = -1;  // Stop dragging
        }
        if (isRotating) {
            isRotating = false;
            rotatingLineIndex = -1;
            if (HasCapture()) ReleaseMouse();
        }
    }
};

class MyFrame final : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Geometric Constraint Visualizer", wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
        wxToolBar* toolbar = wxFrame::CreateToolBar(wxTB_TEXT | wxTB_NOICONS);
        toolbar->AddTool(ID_Point, "Point", wxNullBitmap);
        toolbar->AddTool(ID_Line, "Line", wxNullBitmap);
        toolbar->AddTool(ID_Constraint, "Constraint", wxNullBitmap);
        toolbar->Realize();

        Bind(wxEVT_TOOL, &MyFrame::OnPoint, this, ID_Point);
        Bind(wxEVT_TOOL, &MyFrame::OnLine, this, ID_Line);
        Bind(wxEVT_TOOL, &MyFrame::OnConstraint, this, ID_Constraint);

        canvas = new MyCanvas(this);
    }

private:
    MyCanvas* canvas;

    void OnPoint(wxCommandEvent&) {
        CoordinateDialog dlg(this, "Enter Point Coordinates");
        if (dlg.ShowModal() == wxID_OK) {
            const double x = dlg.GetX();
            const double y = dlg.GetY();

            // Create a GCS point and add it to the canvas
            const Point point(x, y);
            canvas->AddPoint(std::make_shared<Point>(point));
        }
    }

    void OnLine(wxCommandEvent&) {
        CoordinateDialog dlg1(this, "Enter Start Point Coordinates");
        if (dlg1.ShowModal() == wxID_OK) {
            const double x1 = dlg1.GetX();
            const double y1 = dlg1.GetY();

            CoordinateDialog dlg2(this, "Enter End Point Coordinates");
            if (dlg2.ShowModal() == wxID_OK) {
                const double x2 = dlg2.GetX();
                const double y2 = dlg2.GetY();

                // Create a GCS line and add it to the canvas
                const Point startPoint(x1, y1);
                const Point endPoint(x2, y2);
                const Line line = Line::fromTwoPoints(startPoint, endPoint);

                canvas->AddLine(std::make_shared<Line>(line));
            }
        }
    }

    void OnConstraint(wxCommandEvent&) {
        const std::vector types = {
            ConstraintType::P2PHorizontal,
            ConstraintType::P2PVertical,
            ConstraintType::P2PDistance,
            ConstraintType::P2LDistance,
            ConstraintType::L2LParallel,
            ConstraintType::L2LDistance
        };

        wxArrayString choices;
        for (const ConstraintType t : types) {
            choices.Add(ConstraintTypeToString(t));
        }

        wxSingleChoiceDialog typeDialog(this, "Select Constraint Type", "Constraint", choices);
        if (typeDialog.ShowModal() != wxID_OK) return;

        wxString selectedStr = typeDialog.GetStringSelection();
        auto it = std::ranges::find_if(types, [&](ConstraintType t) {
            return ConstraintTypeToString(t) == selectedStr;
        });

        if (it == types.end()) {
            wxMessageBox("Unknown constraint type selected", "Error", wxOK | wxICON_ERROR);
            return;
        }

        ConstraintType selected = *it;
        auto points = canvas->GetPoints();
        auto lines = canvas->GetLines();

        if (selected == ConstraintType::P2LDistance) {
            if (points.empty() || lines.empty()) {
                wxMessageBox("At least one line and one point are required to add a constraint.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        } else if (selected == ConstraintType::L2LParallel || selected == ConstraintType::L2LDistance) {
            if (lines.size() < 2) {
                wxMessageBox("At least two lines are required to add a constraint.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        } else {
            if (points.size() < 2) {
                wxMessageBox("At least two points are required to add a constraint.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }

        if (selected == ConstraintType::L2LDistance) {
            L2LDistanceConstraintDialog dialog(this, lines);
            if (dialog.ShowModal() == wxID_OK) {
                int i1 = dialog.GetFirstIndex();
                int i2 = dialog.GetSecondIndex();
                double d = dialog.GetDistance();
                if (i1 == wxNOT_FOUND || i2 == wxNOT_FOUND || i1 == i2 || d <= 0) {
                    wxMessageBox("Invalid input for distance constraint.", "Invalid Selection", wxOK | wxICON_WARNING);
                    return;
                }
                auto line1 = canvas->GetLines()[i1];
                auto line2 = canvas->GetLines()[i2];
                canvas->AddConstraint(std::make_unique<L2LDistanceConstraint>(line1, line2, d));
            }
        } else if (selected == ConstraintType::L2LParallel) {
            L2LParallelConstraintDialog dialog(this, lines);
            if (dialog.ShowModal() == wxID_OK) {
                int i1 = dialog.GetFirstIndex();
                int i2 = dialog.GetSecondIndex();
                if (i1 == wxNOT_FOUND || i2 == wxNOT_FOUND || i1 == i2) {
                    wxMessageBox("Please select two different lines.", "Invalid Selection", wxOK | wxICON_WARNING);
                    return;
                }
                auto line1 = canvas->GetLines()[i1];
                auto line2 = canvas->GetLines()[i2];
                canvas->AddConstraint(std::make_unique<L2LParallelConstraint>(line1, line2));
            }
        } else if (selected == ConstraintType::P2PDistance) {
            P2PDistanceConstraintDialog dialog(this, points);
            if (dialog.ShowModal() == wxID_OK) {
                int i1 = dialog.GetFirstIndex();
                int i2 = dialog.GetSecondIndex();
                double d = dialog.GetDistance();

                if (i1 == wxNOT_FOUND || i2 == wxNOT_FOUND || i1 == i2 || d <= 0) {
                    wxMessageBox("Invalid input for distance constraint.", "Error", wxOK | wxICON_WARNING);
                    return;
                }

                auto constraint = std::make_unique<P2PDistanceConstraint>(points[i1], points[i2], d);
                canvas->AddConstraint(std::move(constraint));
                canvas->Refresh();
            }
        } else if (selected == ConstraintType::P2LDistance) {
            P2LDistanceConstraintDialog dialog(this, points, lines);
            if (dialog.ShowModal() == wxID_OK) {
                int i1 = dialog.GetPointIndex();
                int i2 = dialog.GetLineIndex();
                double d = dialog.GetDistance();

                if (i1 == wxNOT_FOUND || i2 == wxNOT_FOUND || d <= 0) {
                    wxMessageBox("Invalid input for distance constraint.", "Error", wxOK | wxICON_WARNING);
                    return;
                }

                auto constraint = std::make_unique<P2LDistanceConstraint>(points[i1], lines[i2], d);
                canvas->AddConstraint(std::move(constraint));
                canvas->Refresh();
            }
        } else {
            P2PConstraintDialog dialog(this, points);
            if (dialog.ShowModal() == wxID_OK) {
                int i1 = dialog.GetFirstIndex();
                int i2 = dialog.GetSecondIndex();

                if (i1 == wxNOT_FOUND || i2 == wxNOT_FOUND || i1 == i2) {
                    wxMessageBox("Please select two different points.", "Invalid Selection", wxOK | wxICON_WARNING);
                    return;
                }

                if (selected == ConstraintType::P2PHorizontal) {
                    auto constraint = std::make_unique<P2PHorizontalConstraint>(points[i1], points[i2]);
                    canvas->AddConstraint(std::move(constraint));
                } else if (selected == ConstraintType::P2PVertical) {
                    auto constraint = std::make_unique<P2PVerticalConstraint>(points[i1], points[i2]);
                    canvas->AddConstraint(std::move(constraint));
                }

                canvas->Refresh();
            }
        }
    }
};

class MyApp final : public wxApp {
public:
    bool OnInit() override {
        auto* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);