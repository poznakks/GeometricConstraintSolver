#include <wx/wx.h>
#include "objects/point.h"
#include "objects/line.h"
#include <cmath>

enum ID {
    ID_Point = 1,
    ID_Line,
    ID_Constraint
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
    }

    void AddPoint(const Point& gcs_point) {
        points.push_back(gcs_point);
        Refresh();  // Redraw the canvas
    }

    void AddLine(const Line& gcs_line) {
        lines.push_back(gcs_line);
        Refresh();  // Redraw the canvas
    }

    std::vector<Point> getPoints() const {
        return points;
    }

private:
    std::vector<Point> points;  // Points from GCS
    std::vector<Line> lines;    // Lines from GCS
    bool isDragging = false;
    unsigned long draggingPointIndex = -1;  // To track the point being dragged
    unsigned long draggingLineIndex = -1;   // To track the line being dragged
    wxPoint lastMousePos;

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
            constexpr int length = 10;
            wxPoint p1(line.point.x - line.direction.x * length, line.point.y - line.direction.y * length);
            wxPoint p2(line.point.x + line.direction.x * length, line.point.y + line.direction.y * length);
            dc.DrawLine(p1, p2);
        }
    }

    // Helper function to check if a click is near a line
    static bool IsNearLine(const Line& line, const wxPoint& pos, const double threshold = 5.0) {
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
            Refresh();  // Redraw canvas with updated point or line position
        }
    }

    void OnLeftUp(wxMouseEvent&) {
        isDragging = false;
        draggingPointIndex = -1;
        draggingLineIndex = -1;  // Stop dragging
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
        wxLogMessage("Add constraint functionality to be implemented");
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