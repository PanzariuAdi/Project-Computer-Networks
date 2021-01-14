struct Problem {
    char problemText[300];
    char problemInput[100];
    char problemOutput[100];
    int problemID;
}clientProblem, problems[100], serverProblem;

struct Clasament {
    int idStudent;
    int idProblem;
    int punctaj;
    char studentName[50];

} clasamentFinal[50];