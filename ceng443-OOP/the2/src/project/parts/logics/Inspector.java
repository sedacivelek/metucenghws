package project.parts.logics;

import project.SimulationRunner;
import project.components.Robot;
import project.parts.Arm;
import project.parts.payloads.Payload;
import project.utility.Common;

import java.util.List;

public class Inspector extends Logic
{
    @Override public void run ( Robot robot )
    {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf( "Robot %02d : Detected a broken robot (%02d), adding it to broken robots list.%n", ...);
        // System.out.printf( "Robot %02d : Notifying waiting fixers.%n", ...);
        boolean isBroken = false;
        Class base = robot.getClass();
        if(base.getName().equals("project.parts.Base")){
            //retrieve serialNo with reflection
            int serialNo = (int) Common.get(robot, "serialNo");
            //retrieve robots
            List<Robot> robots = SimulationRunner.factory.robots;
            //retrieve broken robots
            List<Robot> brokenRobots = SimulationRunner.factory.brokenRobots;
            //search for broken robot
            for(Robot brokenRobot:robots){
                synchronized (brokenRobot){
                    int brokenSerialNo = (int) Common.get(brokenRobot,"serialNo");
                    Arm brokenArm = (Arm) Common.get(brokenRobot,"arm");
                    Payload brokenPayload = (Payload) Common.get(brokenRobot,"payload");
                    Logic brokenLogic = (Logic) Common.get(brokenRobot,"logic");
                    //if one part of robot is broken add it to broken robots
                    if(brokenArm==null || brokenPayload==null || brokenLogic==null){
                        if(!brokenRobots.contains(brokenRobot)){
                            synchronized (System.out){
                                System.out.printf( "Robot %02d : Detected a broken robot (%02d), adding it to broken robots list.%n",serialNo, brokenSerialNo);
                            }
                            synchronized (brokenRobots){
                                brokenRobots.add(brokenRobot);
                            }
                            isBroken = true;
                        }
                    }
                }
            }
            //if broken robot is found, notify fixers
            if(isBroken){
                synchronized (System.out){
                    System.out.printf( "Robot %02d : Notifying waiting fixers.%n",serialNo);
                }
                synchronized (brokenRobots){
                    brokenRobots.notifyAll();
                }
            }

        }
    }
}