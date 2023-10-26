#include <iostream>
#include <wx/wx.h>

# define MAX_SIZE 100

using namespace std;

bool isOperator(char);
int _count(wxString, char);
int _toDigit(char);
bool isInvalid(wxString);
wxString toPostfix(wxString);
double evalPostfix(wxString);

template <typename T>
class Stack {
private:
	int top = -1;
public:
	T elements[MAX_SIZE];

	void push(T element) {
		if (top == MAX_SIZE - 1) return;

		elements[++top] = element;
	}

	T pop() {
		if (top == -1) return NULL;

		return elements[top--];
	}

	T peek() {
		if (top == -1) return NULL;

		return elements[top];
	}
};

class App : public wxApp {
private:
	wxWindow* window;
	wxTextCtrl* expressionInput;
    wxStaticText* postfixLabel;
    wxStaticText* expressionValLabel;

	void onButtonClicked(wxCommandEvent& event) {
		wxString expression = expressionInput->GetValue();

        expression.Replace(" ", wxEmptyString);

        if (isInvalid(expression)) {
            wxMessageDialog* dialog = new wxMessageDialog(
                window, 
                "Invalid Expression!", 
                "Error", 
                wxICON_ERROR | wxOK_DEFAULT
            );
            dialog->Centre();
            dialog->ShowModal();
            expressionInput->SetFocus();
            return;
        }

        wxString postFix = toPostfix(expression);
        postfixLabel->SetLabel("Expression in Postfix: " + postFix);
        expressionValLabel->SetLabel(wxString::Format("Expression Value: %.2f", evalPostfix(postFix)));

        window->SetFocus();
	}
public:
	bool OnInit() {
		window = new wxFrame(
			nullptr,
			wxID_ANY,
			"Arithmetic Expression Evaluator",
			wxDefaultPosition,
			wxDefaultSize,
			wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX
		);
		window->SetClientSize(wxSize(800, 600));

		wxPanel* panel = new wxPanel(window, wxID_ANY);

		wxStaticText* title = new wxStaticText(
			panel,
			wxID_ANY,
			"Arithmetic Expression Evaluator",
			wxPoint(100, 50)
		);
		title->SetFont(window->GetFont().Bold().Scale(3.35));

		wxStaticText* info = new wxStaticText(
			panel,
			wxID_ANY,
			"> This Tool evaluates the expression using Stack, first it converts the infix expression to postfix then it evaluates the postfix expression.\nNote: It only supports only one (1) digit numbers currently.",
			wxPoint(25, 125)
		);
		info->Wrap(500);
		info->SetFont(window->GetFont().Scale(1.5));

		wxStaticText* expressionLabel = new wxStaticText(panel, wxID_ANY, "Expression: ", wxPoint(200, 225));
		expressionLabel->SetFont(window->GetFont().Scale(2.5));

		expressionInput = new wxTextCtrl(
			panel,
			wxID_ANY,
			wxEmptyString,
			wxPoint(205 + expressionLabel->GetSize().GetWidth(), 225),
			wxSize(400, expressionLabel->GetSize().GetHeight()),
			wxDEFAULT | wxTE_PROCESS_ENTER
		);
		expressionInput->SetFont(window->GetFont().Scale(2));
		expressionInput->Bind(wxEVT_TEXT_ENTER, &App::onButtonClicked, this);

		postfixLabel = new wxStaticText(panel, wxID_ANY, "Expression in Postfix: ______", wxPoint(200, 300));
		postfixLabel->SetFont(window->GetFont().Scale(2.5));

		expressionValLabel = new wxStaticText(panel, wxID_ANY, "Expression Value: ___", wxPoint(200, 375));
		expressionValLabel->SetFont(window->GetFont().Scale(2.5));

		wxButton* button = new wxButton(
			panel,
			wxID_ANY,
			"Evaluate Expression",
			wxPoint(250, 475),
			wxSize(300, 50)
		);
		button->SetFont(window->GetFont().Scale(2.5));
		button->Bind(wxEVT_BUTTON, &App::onButtonClicked, this);

		window->Center();
		window->Show();

		return true;
	}
};

bool isOperator(char ch) {
    switch (ch) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
            return true;

        default:
            return false;
    }
}

int _count(wxString string, char ch) {
    int count = 0;

    for (int i = 0; i < string.length(); i++) {
        if (string[i] == ch) count++;
    }

    return count;
}

int _toDigit(char ch) {
    switch (ch) {
        case '0':
            return 0;

        case '1':
            return 1;

        case '2':
            return 2;

        case '3':
            return 3;

        case '4':
            return 4;

        case '5':
            return 5;

        case '6':
            return 6;

        case '7':
            return 7;

        case '8':
            return 8;

        case '9':
            return 9;

        default:
            return NULL;
    }
}

bool isInvalid(wxString str) {
    if (str.length() < 3) return true;

    if (_count(str, '(') != _count(str, ')')) return true;

    bool invaild = false;

    int i = 0;
    while (i < str.length()) {
        if (str[i] == '(') {
            int j = 1, numofClosing = 1;

            while (i + j < str.length()) {
                if (numofClosing == 0) break;

                if (str[i + j] == '(') {
                    numofClosing++;
                } else if (str[i + j] == ')') {
                    numofClosing--;
                }

                j++;
            }

            if (numofClosing != 0) {
                invaild = true;
                break;
            }

        } else if (str[i] >= 48 && str[i] <= 57) {
            if (i + 1 == str.length()) break;

            if (str[i + 1] == ')') {
                i++;
                continue;
            }

            if (isOperator(str[i + 1]) && ((str[i + 2] >= 48 && str[i + 2] <= 57) || str[i + 2] == '(')) {
                i += 2;
                continue;
            } else {
                invaild = true;
                break;
            }
        } else if (str[i] == ')') {
            if (i + 1 == str.length()) break; 


            if (str[i + 1] == ')') {
                i++;
                continue;
            }

            if ((isOperator(str[i + 1]) || str[i + 1] == ')') 
                && ((str[i + 2] >= 48 && str[i + 2] <= 57) || str[i + 2] == '(')) {
                i += 2;
                continue;
            } else {
                invaild = true;
                break;
            }

        } else {
            invaild = true;
            break;
        }

        i++;
    }


    return invaild;
}

wxString toPostfix(wxString expr) {
    expr.insert(expr.begin(), '(');
    expr.Append(')');

    Stack<char> stack;
    wxString out = wxEmptyString;
    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == '(') {
            stack.push(expr[i]);

        } else if (expr[i] >= 48 && expr[i] <= 57) {
            out.Append(expr[i]);

        } else if (expr[i] == '^') {
            if (stack.peek() == '^') {
                out.Append(stack.pop());
            }
            stack.push(expr[i]);

        } else if (expr[i] == '*' || expr[i] == '/' || expr[i] == '%') {
            if (stack.peek() != '(' && stack.peek() != '+' && stack.peek() != '-') {
                out.Append(stack.pop());
            }
            stack.push(expr[i]);

        } else if (expr[i] == '+' || expr[i] == '-') {
            if (stack.peek() != '(') {
                out.Append(stack.pop());
            }
            stack.push(expr[i]);

        } else if (expr[i] == ')') {
            char poppedVal = stack.pop();
            while (poppedVal != '(') {
                out.Append(poppedVal);
                poppedVal = stack.pop();
            }
        }
    }

    return out;
}

double evalPostfix(wxString expr) {
    Stack<double> stack; 
    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] >= 48 && expr[i] <= 57) {
            stack.push(_toDigit(expr[i]));

        } else if (expr[i] == '+') {
            double num1 = stack.pop(), num2 = stack.pop();
            stack.push(num2 + num1);

        } else if (expr[i] == '-') {
            double num1 = stack.pop(), num2 = stack.pop();
            stack.push(num2 - num1);

        } else if (expr[i] == '*') {
            double num1 = stack.pop(), num2 = stack.pop();
            stack.push(num2 * num1);

        } else if (expr[i] == '/') {
            double num1 = stack.pop(), num2 = stack.pop();
            stack.push(num2 / num1);

        } else if (expr[i] == '%') {
            int num1 = stack.pop(), num2 = stack.pop();
            stack.push(num2 % num1);

        } else if (expr[i] == '^') {
            double num1 = stack.pop(), num2 = stack.pop();
            stack.push(pow(num2, num1));
        }
    }

    return stack.pop();
}

IMPLEMENT_APP(App);