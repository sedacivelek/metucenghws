import java.io.IOException;
import java.util.stream.Stream;

public class Runner {
    public static void main ( String [] args ) throws IOException {
        SIS informationSystem = new SIS();
        double grade = informationSystem.getGrade(5021430,2300105,20201);
        //informationSystem.updateExam(5021430,5710491,"Midterm2",100);
        grade =informationSystem.getGrade(5021430,2300105,20201);
        System.out.println(grade);
        informationSystem.findCourse(5710336);
        informationSystem.createHistogram(5710336,20222);
        informationSystem.createTranscript(5021430);
    }
}
