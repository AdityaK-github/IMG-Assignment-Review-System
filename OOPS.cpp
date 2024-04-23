#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

class Reviewer;

class IMG_Member
{
protected:
    string name;
    string email;

public:
    IMG_Member(const string &name, const string &email) : name(name), email(email) {}
    string getName() const
    {
        return name;
    }
    virtual void getProfile() const = 0;
    virtual ~IMG_Member() {}
};

class Student : public IMG_Member
{
private:
    vector<string> pendingAssignments;
    map<string, map<string, string>> suggestedIterations;
    vector<string> reviewedAssignments;

public:
    Student(const string &name, const string &email) : IMG_Member(name, email) {}
    void getProfile() const override
    {
        cout << "Student Name: " << getName() << endl;
        cout << "Email: " << email << endl;
    }
    void addAssignment(const string &assignment)
    {
        pendingAssignments.push_back(assignment);
    }
    void addReviewedAssignment(const string &assignment)
    {
        reviewedAssignments.push_back(assignment);
    }
    void submitAssignment(const string &assignment, Reviewer &reviewer);
    bool hasAssignment(const string &assignment) const
    {
        return find(pendingAssignments.begin(), pendingAssignments.end(), assignment) != pendingAssignments.end();
    }
    void removeAssignment(const string &assignment)
    {
        pendingAssignments.erase(remove(pendingAssignments.begin(), pendingAssignments.end(), assignment), pendingAssignments.end());
    }
    bool hasPendingAssignments() const
    {
        return !pendingAssignments.empty();
    }
    bool hasReviewedAssignments() const
    {
        return !reviewedAssignments.empty();
    }
    void suggestedIteration(const string &assignment, const string &suggestion)
    {
        suggestedIterations[assignment][getName()] = suggestion;
        cout << "Suggestion added for " << getName() << " for the assignment: " << assignment << " --> " << suggestion << endl;
    }
    void displaySuggestedIterations(const string &assignment) const
    {
        cout << "Suggested Iterations for " << assignment << " to " << getName() << ":" << endl;
        auto it = suggestedIterations.find(assignment);
        if (it != suggestedIterations.end())
        {
            const auto &iterations = it->second;
            for (const auto &pair : iterations)
            {
                cout << "Student: " << pair.first << ", Suggestion: " << pair.second << endl;
            }
        }
        else
        {
            cout << "No suggested iterations for this assignment." << endl;
        }
    }
    void markAssignmentAsCompleted(const string &assignment)
    {
        if (hasAssignment(assignment))
        {
            reviewedAssignments.push_back(assignment);
            pendingAssignments.erase(remove(pendingAssignments.begin(), pendingAssignments.end(), assignment), pendingAssignments.end());
            cout << "Assignment marked as completed!" << endl;
        }
        else
        {
            cout << "Assignment not found in pending list!" << endl;
        }
    }
    const vector<string> &getPendingAssignments() const
    {
        if (hasPendingAssignments())
        {
            return pendingAssignments;
        }
        else
        {
            static const vector<string> emptyVector;
            return emptyVector;
        }
    }
    const vector<string> &getReviewedAssignments()
    {
        if (hasReviewedAssignments())
        {
            return reviewedAssignments;
        }
        else
        {
            static const vector<string> emptyVector;
            return emptyVector;
        }
    }
    ~Student() {}
};

class Reviewer : public IMG_Member
{
public:
    vector<string> pendingAssignments;
    Reviewer(const string &name, const string &email) : IMG_Member(name, email) {}
    void getProfile() const override
    {
        cout << "Reviewer Name: " << getName() << endl;
        cout << "Email: " << email << endl;
    }
    void addPendingAssignment(const string &assignment)
    {
        pendingAssignments.push_back(assignment);
    }
    void getPendingAssignment()
    {
        for (auto &assignment : pendingAssignments)
        {
            cout << assignment << endl;
        }
    }
    void reviewAssignment(const string &assignment, Student &student)
    {
        cout << "Assignment reviewed and marked as 'completed!' by " << getName() << endl;
        student.removeAssignment(assignment);
    }
    void addAssignment(const string &assignment, vector<Student> &students)
    {
        for (Student &student : students)
        {
            student.addAssignment(assignment);
        }
        cout << "Assignment added for all students!" << endl;
    }
    void suggestIteration(Student &student, const string &assignment, const string &suggestion)
    {
        student.suggestedIteration(assignment, suggestion);
    }
    ~Reviewer() {}
};

void Student::submitAssignment(const string &assignment, Reviewer &reviewer)
{
    cout << "Assignment submitted successfully!" << endl;
    reviewer.addPendingAssignment(assignment);
}

class AssignmentReviewSystem
{
private:
    vector<Student> students;
    vector<Reviewer> reviewers;

public:
    vector<Reviewer> &getReviewers()
    {
        return reviewers;
    }
    vector<Student> &getStudents()
    {
        return students;
    }
    void addStudent(const Student &student)
    {
        students.push_back(student);
    }
    void addReviewer(const Reviewer &reviewer)
    {
        reviewers.push_back(reviewer);
    }
    void displayPendingAssignments(const Student &student) const
    {
        cout << "Pending Assignments: \n";
        for (const auto &assignment : student.getPendingAssignments())
            cout << assignment << endl;
    }
    void displayReviewedAssignments(Student &student)
    {
        cout << "Reviewed Assignments: ";
        for (const auto &assignment : student.getReviewedAssignments())
            cout << assignment << endl;
    }
    void displayStudentsWithPendingAssignments() const
    {
        cout << "Students with pending assignments:" << endl;
        for (const auto &student : students)
        {
            if (student.hasPendingAssignments())
            {
                cout << "- " << student.getName() << endl;
            }
        }
    }
    void displayPendingAssignmentsForReviewer() const
    {
        cout << "Pending assignments for review:" << endl;
        for (const auto &student : students)
        {
            if (student.hasPendingAssignments())
            {
                cout << "Student: " << student.getName() << endl;
                for (const auto &assignment : student.getPendingAssignments())
                {
                    cout << "  - " << assignment << endl;
                }
            }
        }
    }
    void reviewAssignment(Reviewer &reviewer, Student &student, const string &assignment)
    {
        if (student.hasAssignment(assignment))
        {
            reviewer.reviewAssignment(assignment, student);
        }
        else
        {
            cout << "Assignment not found for " << student.getName() << endl;
        }
    }
    void suggestIteration(Student &student, const string &assignment, const string &suggestion)
    {
        student.suggestedIteration(assignment, suggestion);
    }
    ~AssignmentReviewSystem() {}
};

int main()
{
    AssignmentReviewSystem system;

    while (true)
    {
        cout << "\nMain Menu\n";
        cout << "1) Add Student\n";
        cout << "2) Add Reviewer\n";
        cout << "3) Login as a Student or Reviewer\n";
        cout << "4) Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (cin.fail())
        {
            cout << "Invalid input! Please enter a valid option." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            string name, email;
            cout << "Enter Student Name: ";
            getline(cin, name);
            cout << "Enter Student Email: ";
            getline(cin, email);
            Student student(name, email);
            system.addStudent(student);
            break;
        }
        case 2:
        {
            string name, email;
            cout << "Enter Reviewer Name: ";
            getline(cin, name);
            cout << "Enter Reviewer Email: ";
            getline(cin, email);
            Reviewer reviewer(name, email);
            system.addReviewer(reviewer);
            break;
        }
        case 3:
        {
            cout << "\n1) Login as Student\n";
            cout << "2) Login as Reviewer\n";
            cout << "Choose an option: ";
            int loginChoice;
            cin >> loginChoice;
            cin.ignore();

            if (loginChoice == 1)
            {
                string studentName;
                cout << "Enter Student Name: ";
                getline(cin, studentName);

                bool found = false;
                for (Student &student : system.getStudents())
                {
                    if (student.getName() == studentName)
                    {
                        found = true;
                        if (found)
                        {
                            while (true)
                            {
                                cout << "\nStudent Menu\n";
                                cout << "1) Display Pending Assignments\n";
                                cout << "2) Display Reviewed Assignments\n";
                                cout << "3) Submit Assignment\n";
                                cout << "4) See suggested iterations\n";
                                cout << "5) Go Back to Main Menu\n";
                                cout << "Choose an option: ";

                                int studentOption;
                                cin >> studentOption;
                                cin.ignore();

                                string assignmentName, reviewerName, reviewerEmail;

                                switch (studentOption)
                                {
                                case 1:
                                    system.displayPendingAssignments(student);
                                    cout << endl;
                                    break;
                                case 2:
                                    system.displayReviewedAssignments(student);
                                    break;
                                case 3:
                                {
                                    if (student.hasPendingAssignments())
                                    {
                                        cout << "Enter Assignment Name: ";
                                        getline(cin, assignmentName);
                                        cout << "Enter Reviewer Name: ";
                                        getline(cin, reviewerName);
                                        for (Reviewer &reviewer : system.getReviewers())
                                        {
                                            if (reviewer.getName() == reviewerName)
                                            {
                                                reviewer.addPendingAssignment(assignmentName);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        cout << "You don't have any pending assignments!" << endl;
                                    }
                                    break;
                                }
                                case 4:
                                    cout << "Enter Assignment Name: ";
                                    getline(cin, assignmentName);
                                    student.displaySuggestedIterations(assignmentName);
                                    break;
                                case 5:
                                    goto Main_Menu;
                                default:
                                    cout << "Invalid Option! Try Again." << endl;
                                }
                            }
                        }
                    }
                }
                if (!found)
                    cout << "Student not found!" << endl;
            }
            else if (loginChoice == 2)
            {
                string reviewerName;
                cout << "Enter reviewer's name: ";
                getline(cin, reviewerName);
                for (Reviewer &reviewer : system.getReviewers())
                {
                    if (reviewer.getName() == reviewerName)
                    {
                        while (true)
                        {
                            int choice;
                            cout << "\nReviewer's Menu:" << endl;
                            cout << "1)Add assignments for students\n";
                            cout << "2)Review assignments\n";
                            cout << "3)Suggest iterations for an assignment\n";
                            cout << "4)Display students with pending assignments\n";
                            cout << "5)Go back to main menu\n";
                            cout << "Choose an option: ";
                            cin >> choice;
                            cin.ignore();

                            string assignmentName;
                            string studentName;
                            string suggestion;
                            switch (choice)
                            {
                            case 1:
                                cout << "\nEnter assignment name: ";
                                getline(cin, assignmentName);
                                for (Student &student : system.getStudents())
                                {
                                    student.addAssignment(assignmentName);
                                }
                                break;
                            case 2:
                                reviewer.getPendingAssignment();
                                cout << "Choose assignment name: ";
                                getline(cin, assignmentName);
                                cout << "Students who haven't completed this assignment are: " << endl;
                                for (Student &student : system.getStudents())
                                {
                                    if (student.hasPendingAssignments())
                                    {
                                        if (find(student.getPendingAssignments().begin(), student.getPendingAssignments().end(), assignmentName) != student.getPendingAssignments().end())
                                        {
                                            cout << student.getName() << endl;
                                        }
                                    }
                                }
                                cout << "Enter student name to review the assignment for: ";
                                getline(cin, studentName);
                                for (Student &student : system.getStudents())
                                {
                                    if (studentName == student.getName())
                                    {
                                        reviewer.reviewAssignment(assignmentName, student);
                                    }
                                }
                                break;
                            case 3:
                                if (reviewer.pendingAssignments.size() != 0)
                                {
                                    cout << "Enter student name:";
                                    getline(cin, studentName);
                                    cout << "Enter assignment name:";
                                    getline(cin, assignmentName);
                                    cout << "Enter your suggestion: ";
                                    getline(cin, suggestion);
                                    for (Student &student : system.getStudents())
                                    {
                                        for (string assignment : student.getPendingAssignments())
                                        {
                                            if (assignment == assignmentName && studentName == student.getName())
                                            {
                                                reviewer.suggestIteration(student, assignment, suggestion);
                                            }
                                        }
                                    }
                                }
                                else {
                                    cout << "No pending assignments!! :)\n\n";
                                }
                                break;
                            case 4:
                                cout << "Students with pending assignments: " << endl;
                                for (Student student : system.getStudents())
                                {
                                    if (student.hasPendingAssignments())
                                    {
                                        cout << student.getName() << endl;
                                    }
                                }
                                break;
                            case 5:
                                goto Main_Menu;
                            default:
                                cout << "Invalid choice! Try again." << endl;
                            }
                        }
                    }
                }
            }
            else
            {
                cout << "Invalid Option! Try Again." << endl;
            }
            break;
        }
        case 4:
            return 0;
        default:
            cout << "Invalid Option! Try Again." << endl;
        }
    Main_Menu:;
    }
    return 0;
}