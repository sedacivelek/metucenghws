
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.function.BinaryOperator;
import java.util.function.Function;
import java.util.stream.Collectors;

import static java.util.stream.Collectors.groupingBy;

public class SIS {
    private static String fileSep = File.separator;
    private static String lineSep = System.lineSeparator();
    private static String space   = " ";

    private List<Student> studentList = new ArrayList<>();

    public SIS() throws IOException { processOptics(); }

    //process all input files and add students to student list
    private void processOptics() throws IOException {
        //first retrieve first line from input files to add to student list
        List<String> students = new ArrayList<>();
        Files.list(Paths.get("input")).forEach(p -> {
            try {
                students.addAll(Files.lines(Paths.get(p.toString())).limit(1).collect(Collectors.toList()));
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
        //for each unique student add to student list by using fields
        List<String> uniqueStudents = students.stream().distinct().collect(Collectors.toList());
        uniqueStudents.stream().forEach((s -> {
            List<String> studentFields = Arrays.stream(s.split(space)).collect(Collectors.toList());
            studentList.add(new Student(studentFields.stream().limit(studentFields.size()-2).toArray(String[]::new), studentFields.get(studentFields.size()-2), Integer.parseInt(studentFields.get(studentFields.size()-1))));
        }));
        //retrieve all files again to find course information of the courses that are taken by student, add to student with corresponding surname
        Files.list(Paths.get("input")).forEach(path -> {
            try {
                //student infos
                List<String> firstLine = Arrays.stream(Files.lines(Paths.get(path.toString())).limit(1).collect(Collectors.toList()).get(0).split(space)).collect(Collectors.toList());
                //year-coursecode-credit info
                List<String> secondLine = Arrays.stream(Files.lines(Paths.get(path.toString())).skip(1).limit(1).collect(Collectors.toList()).get(0).split(space)).collect(Collectors.toList());
                //examtype info
                List<String> thirdLine = Arrays.stream(Files.lines(Paths.get(path.toString())).skip(2).limit(1).collect(Collectors.toList()).get(0).split(space)).collect(Collectors.toList());
                //submitted answers info
                List<String> forthLine = Arrays.stream(Files.lines(Paths.get(path.toString())).skip(3).limit(1).collect(Collectors.toList()).get(0).split(space)).collect(Collectors.toList());
                //count true answers
                long trueAnswers= forthLine.get(0).chars().filter(ch -> ch =='T').count();
                //count all answers
                long allAnswers = forthLine.get(0).chars().count();
                //calculate grade
                double grade = (100.0*trueAnswers/allAnswers);
                //find student with corresponding student id
                Student student =studentList.stream().filter(s -> s.getStudentID()==(Integer.parseInt(firstLine.get(firstLine.size()-1)))).findFirst().orElse(null);
                //add course to takencourses list of student
                student.getTakenCourses().add(new Course(Integer.parseInt(secondLine.get(1)),Integer.parseInt(secondLine.get(0)),thirdLine.get(0),Integer.parseInt(secondLine.get(2)),grade));
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

    }
    //Returns student's overall grade for the course offered in the year
    public double getGrade(int studentID, int courseCode, int year){
        // filter student list by given student id
        Student student = studentList.stream().filter(s -> s.getStudentID()==studentID).findFirst().orElse(null);
        //filter taken courses by given courseCode and year
        List<Course> courses=student.getTakenCourses().stream().filter(c->c.getCourseCode()==courseCode).filter(c->c.getYear()==year).collect(Collectors.toList());
        //retrieve corresponding exams with filter by examType, retrieve grades
        double mt1 = courses.stream().filter(c->c.getExamType().equals("Midterm1")).findFirst().orElse(null).getGrade();
        double mt2 = courses.stream().filter(c->c.getExamType().equals("Midterm2")).findFirst().orElse(null).getGrade();
        double finalEx = courses.stream().filter(c->c.getExamType().equals("Final")).findFirst().orElse(null).getGrade();
        //calculate overall grade
        double grade = 0.25*mt1+0.25*mt2+0.5*finalEx;
        return grade;
    }

    //Updates student's exam.
    public void updateExam(int studentID, int courseCode, String examType, double newGrade){
        //retrieve student filtering by studentId
        Student student=studentList.stream().filter(s->s.getStudentID()==studentID).findFirst().orElse(null);
        //find course that has given course code examtype, course must be the most recently taken one, set new grade
        student.getTakenCourses().stream().filter(c->c.getCourseCode()==courseCode).filter(c->c.getExamType().equals(examType)).max(Comparator.comparing(Course::getYear)).orElse(null).setGrade(newGrade);
        // TODO
    }

    //Prints the transcript of the student.
    public void createTranscript(int studentID){
        //retrieve student filtering by studentId
        Student student = studentList.stream().filter(s->s.getStudentID()==studentID).findFirst().orElse(null);
        //retrieve courses taken by student sort them first by year then by course code
        List<Course> takenCourses = student.getTakenCourses().stream().sorted(Comparator.comparingInt(Course::getYear).thenComparing(Course::getCourseCode)).filter(c->c.getExamType().equals("Midterm1")).collect(Collectors.toList());
        //find most recently taken courses by year
        Map<Integer,Course> mostRecent = takenCourses.stream().collect(Collectors.toMap(Course::getCourseCode, Function.identity(),
                BinaryOperator.maxBy(Comparator.comparing(Course::getYear))));
        //map all taken courses by year to print courses year by year
        Map<Integer,List<Course>> mappedByYear = takenCourses.stream().sorted(Comparator.comparingInt(Course::getYear).thenComparing(Course::getCourseCode)).collect(groupingBy(Course::getYear));
        Map<Integer,List<Course>> sortedMappedByYear = new TreeMap<>();
        //sort years
        mappedByYear.entrySet().stream().sorted(Map.Entry.comparingByKey()).forEachOrdered(c->sortedMappedByYear.put(c.getKey(),c.getValue()));
        double cgpa =0;
        List<Integer> totalCredit = new ArrayList<>();
        List<Double> totalPoint = new ArrayList<>();
        //for each year
        sortedMappedByYear.entrySet().stream().forEach(t ->{
            //print year
            System.out.println(t.getKey());
            //for each course in that year
            t.getValue().stream().forEach(c->{
                //check if course is recently taken
                Boolean isRecent =!mostRecent.entrySet().stream().filter(y->y.getValue().getYear()==t.getKey()&&y.getKey()==c.getCourseCode()).collect(Collectors.toList()).isEmpty();
                //calculate letter grade
                //if course is recent add totalpoint list to calculate cgpa
                double grade = getGrade(studentID,c.getCourseCode(),t.getKey());
                String letterGrade = "";
                if(grade<49.5){
                    letterGrade="FF";
                    if(isRecent) totalPoint.add(c.getCredit()*0.0);
                }
                else if(grade<59.5){
                    letterGrade="FD";
                    if(isRecent) totalPoint.add(c.getCredit()*0.5);
                }
                else if(grade<64.5){
                    letterGrade="DD";
                    if(isRecent) totalPoint.add(c.getCredit()*1.0);
                }
                else if(grade<69.5){
                    letterGrade="DC";
                    if(isRecent) totalPoint.add(c.getCredit()*1.5);
                }
                else if(grade<74.5){
                    letterGrade="CC";
                    if(isRecent) totalPoint.add(c.getCredit()*2.0);
                }
                else if(grade<79.5){
                    letterGrade="CB";
                    if(isRecent) totalPoint.add(c.getCredit()*2.5);
                }
                else if(grade<84.5){
                    letterGrade="BB";
                    if(isRecent) totalPoint.add(c.getCredit()*3.0);
                }
                else if(grade<89.5){
                    letterGrade="BA";
                    if(isRecent) totalPoint.add(c.getCredit()*3.5);
                }
                else{
                    letterGrade="AA";
                    if(isRecent) totalPoint.add(c.getCredit()*4.0);
                }
                if(isRecent) totalCredit.add(c.getCredit());
                //print letter grade with course code
                System.out.println(c.getCourseCode()+" "+letterGrade);
            });
        });
        //calculate cgpa by summing totalpoint and dividing the sum of taken course credits
        cgpa = totalPoint.stream().reduce(0.0,Double::sum)/totalCredit.stream().reduce(0,Integer::sum);
        System.out.println("CGPA: "+String.format("%.2f",cgpa));
    }

    //Prints the years when the course was offered as well as the number of registered students
    public void findCourse(int courseCode){
        List<Course> courseEntries = new ArrayList<>();
        //add all taken course entries
        studentList.stream().forEach(student -> courseEntries.addAll(student.getTakenCourses()));
        //filter all entries by given course code sort by year
        List<Course> selectedCourse=courseEntries.stream().filter(course -> course.getCourseCode()==courseCode).sorted(Comparator.comparingInt(Course::getYear)).collect(Collectors.toList());
        //map offered years and registered student count
        Map<Integer,Long> counts =selectedCourse.stream().collect(groupingBy(c->c.getYear(), Collectors.counting()));
        //print offered years and #of registered students
        counts.entrySet().stream().sorted(Map.Entry.comparingByKey()).forEach(c-> System.out.println(c.getKey()+" "+c.getValue()/3));
    }
    //Prints the grade histogram of the registered students in the course offered in the year.
    public void createHistogram(int courseCode, int year){
        List<Integer> studentIds = new ArrayList<>();
        //find students that took course with given coursecode and year
        studentList.stream().forEach(s->{
            List<Course> selectedCourse = s.getTakenCourses().stream().filter(c->c.getCourseCode()==courseCode && c.getYear()==year).collect(Collectors.toList());
            if(!selectedCourse.isEmpty()){
                studentIds.add(s.getStudentID());
            }
        });
        //for each student that took given course, retrieve their grade, put to grade list
        List<Double> grades = new ArrayList<>();
        studentIds.stream().forEach(s->grades.add(getGrade(s,courseCode,year)));
        //map grades by given thresholds
        Map<String,Long> histogram = new TreeMap<>();
        histogram.put("0-10",grades.stream().filter(g->g<10).count());
        histogram.put("10-20",grades.stream().filter(g->g<20 && g>=10).count());
        histogram.put("20-30",grades.stream().filter(g->g<30 && g>=20).count());
        histogram.put("30-40",grades.stream().filter(g->g<40 && g>=30).count());
        histogram.put("40-50",grades.stream().filter(g->g<50 && g>=40).count());
        histogram.put("50-60",grades.stream().filter(g->g<60 && g>=50).count());
        histogram.put("60-70",grades.stream().filter(g->g<70 && g>=60).count());
        histogram.put("70-80",grades.stream().filter(g->g<80 && g>=70).count());
        histogram.put("80-90",grades.stream().filter(g->g<90 && g>=80).count());
        histogram.put("90-100",grades.stream().filter(g->g>=90).count());
        //print given threshold and corresponding number of student
        histogram.entrySet().stream().forEach(h-> System.out.println(h.getKey()+" "+h.getValue()));
    }
}