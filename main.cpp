#include <wx/wx.h>
#include "objects/point.h"
#include "objects/line.h"
#include <cmath>

#include "constraints/constraint.h"
#include "constraints/horizontal_constraint.h"
#include "constraints/vertical_constraint.h"
#include "constraints/distance_constraint.h"

enum ID {
    ID_Point = 1,
    ID_Line,
    ID_Constraint
};

enum class ConstraintType {
    Horizontal,
    Vertical,
    Distance
};

inline wxString ConstraintTypeToString(const ConstraintType type) {
    switch (type) {
        case ConstraintType::Horizontal: return "Horizontal";
        case ConstraintType::Vertical:   return "Vertical";
        case ConstraintType::Distance:   return "Distance";
        default: return "Unknown";
    }
}

inline ConstraintType ConstraintTypeFromString(const wxString& str) {
    if (str == "Horizontal") return ConstraintType::Horizontal;
    if (str == "Vertical")   return ConstraintType::Vertical;
    if (str == "Distance")   return ConstraintType::Distance;
    throw std::invalid_argument("Unknown constraint string: " + std::string(str.mb_str()));
}

class DistanceConstraintDialog final : public wxDialog {
public:
    DistanceConstraintDialog(wxWindow* parent, const std::vector<Point>& points)
        : wxDialog(parent, wxID_ANY, "Add Distance Constraint") {

        auto* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select first point:"), 0, wxALL, 5);
        choice1 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice1, 0, wxALL | wxEXPAND, 5);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select second point:"), 0, wxALL, 5);
        choice2 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice2, 0, wxALL | wxEXPAND, 5);

        for (size_t i = 0; i < points.size(); ++i) {
            wxString label = wxString::Format("Point %d (%d, %d)", (int)i, points[i].x, points[i].y);
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

class ConstraintDialog final : public wxDialog {
public:
    ConstraintDialog(wxWindow* parent, const std::vector<Point>& points)
        : wxDialog(parent, wxID_ANY, "Add Constraint", wxDefaultPosition, wxDefaultSize) {

        auto* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select first point:"), 0, wxALL, 5);
        choice1 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice1, 0, wxALL | wxEXPAND, 5);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Select second point:"), 0, wxALL, 5);
        choice2 = new wxChoice(this, wxID_ANY);
        sizer->Add(choice2, 0, wxALL | wxEXPAND, 5);

        for (size_t i = 0; i < points.size(); ++i) {
            wxString label = wxString::Format("Point %d (%d, %d)", static_cast<int>(i), points[i].x, points[i].y);
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

    void AddPoint(const Point& gcs_point) {
        points.push_back(gcs_point);
        Refresh();  // Redraw the canvas
    }

    void AddLine(const Line& gcs_line) {
        lines.push_back(gcs_line);
        Refresh();  // Redraw the canvas
    }

    void AddConstraint(std::unique_ptr<Constraint> constraint) {
        constraints.push_back(std::move(constraint));
        ApplyConstraints(); // сразу применить при добавлении
    }

    std::vector<Point>& GetPoints() { return points; }
    const std::vector<Point>& GetPoints() const { return points; }

private:
    std::vector<Point> points;  // Points from GCS
    std::vector<Line> lines;    // Lines from GCS
    std::vector<std::unique_ptr<Constraint>> constraints;
    bool isDragging = false;
    bool isRotating = false;
    unsigned long rotatingLineIndex = -1;
    unsigned long draggingPointIndex = -1;  // To track the point being dragged
    unsigned long draggingLineIndex = -1;   // To track the line being dragged
    wxPoint lastMousePos;

    void ApplyConstraints() {
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
            dc.DrawCircle(wxPoint(point.x, point.y), 5);  // GCS points mapped to canvas points
        }

        dc.SetPen(*wxBLUE_PEN);
        // Draw lines
        for (const auto& line : lines) {
            constexpr int length = 1;
            wxPoint p1(line.point.x - line.direction.x * length, line.point.y - line.direction.y * length);
            wxPoint p2(line.point.x + line.direction.x * length, line.point.y + line.direction.y * length);
            dc.DrawLine(p1, p2);
        }
    }

    // Helper function to check if a click is near a line
    static bool IsNearLine(const Line& line, const wxPoint& pos) {
        constexpr double threshold = 5.0;
        const wxPoint p1(line.point.x, line.point.y);
        const wxPoint p2(line.point.x + line.direction.x * 10, line.point.y + line.direction.y * 10);

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
            if (abs(points[i].x - pos.x) < 5 && abs(points[i].y - pos.y) < 5) {
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
        if (isDragging) {
            const wxPoint pos = event.GetPosition();
            const int dx = pos.x - lastMousePos.x;
            const int dy = pos.y - lastMousePos.y;

            // If dragging a point
            if (draggingPointIndex != -1) {
                points[draggingPointIndex].x += dx;
                points[draggingPointIndex].y += dy;
            }

            // If dragging a line
            if (draggingLineIndex != -1) {
                lines[draggingLineIndex].point.x += dx;
                lines[draggingLineIndex].point.y += dy;
            }

            lastMousePos = pos;
            ApplyConstraints();
            Refresh();  // Redraw canvas with updated point or line position
        }

        if (isRotating && rotatingLineIndex != -1) {
            const wxPoint pos = event.GetPosition();

            Line& line = lines[rotatingLineIndex];
            const Point origin(line.point.x, line.point.y);

            double dx1 = lastMousePos.x - origin.x;
            double dy1 = lastMousePos.y - origin.y;
            double dx2 = pos.x - origin.x;
            double dy2 = pos.y - origin.y;

            double angle1 = std::atan2(dy1, dx1);
            double angle2 = std::atan2(dy2, dx2);
            double deltaAngle = angle2 - angle1;

            Point& dir = line.direction;

            // Поворот
            double cosA = std::cos(deltaAngle);
            double sinA = std::sin(deltaAngle);

            double newX = dir.x * cosA - dir.y * sinA;
            double newY = dir.x * sinA + dir.y * cosA;

            dir.x = newX;
            dir.y = newY;

            lastMousePos = pos;
            Refresh();
        }
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
            canvas->AddPoint(point);
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

                canvas->AddLine(line);
            }
        }
    }

    void OnConstraint(wxCommandEvent&) {
        const std::vector<ConstraintType> types = {
            ConstraintType::Horizontal,
            ConstraintType::Vertical,
            ConstraintType::Distance
        };

        wxArrayString choices;
        for (ConstraintType t : types) {
            choices.Add(ConstraintTypeToString(t));
        }

        wxSingleChoiceDialog typeDialog(this, "Select Constraint Type", "Constraint", choices);
        if (typeDialog.ShowModal() != wxID_OK) return;

        wxString selectedStr = typeDialog.GetStringSelection();
        auto it = std::find_if(types.begin(), types.end(), [&](ConstraintType t) {
            return ConstraintTypeToString(t) == selectedStr;
        });

        if (it == types.end()) {
            wxMessageBox("Unknown constraint type selected", "Error", wxOK | wxICON_ERROR);
            return;
        }

        ConstraintType selected = *it;
        auto& points = canvas->GetPoints();

        if (points.size() < 2) {
            wxMessageBox("At least two points are required to add a constraint.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        if (selected == ConstraintType::Distance) {
            DistanceConstraintDialog dialog(this, points);
            if (dialog.ShowModal() == wxID_OK) {
                int i1 = dialog.GetFirstIndex();
                int i2 = dialog.GetSecondIndex();
                double d = dialog.GetDistance();

                if (i1 == wxNOT_FOUND || i2 == wxNOT_FOUND || i1 == i2 || d <= 0) {
                    wxMessageBox("Invalid input for distance constraint.", "Error", wxOK | wxICON_WARNING);
                    return;
                }

                auto constraint = std::make_unique<DistanceConstraint>(points[i1], points[i2], d);
                canvas->AddConstraint(std::move(constraint));
                canvas->Refresh();
            }
        } else {
            ConstraintDialog dialog(this, points);
            if (dialog.ShowModal() == wxID_OK) {
                int i1 = dialog.GetFirstIndex();
                int i2 = dialog.GetSecondIndex();

                if (i1 == wxNOT_FOUND || i2 == wxNOT_FOUND || i1 == i2) {
                    wxMessageBox("Please select two different points.", "Invalid Selection", wxOK | wxICON_WARNING);
                    return;
                }

                if (selected == ConstraintType::Horizontal) {
                    auto constraint = std::make_unique<HorizontalConstraint>(points[i1], points[i2]);
                    canvas->AddConstraint(std::move(constraint));
                } else if (selected == ConstraintType::Vertical) {
                    auto constraint = std::make_unique<VerticalConstraint>(points[i1], points[i2]);
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