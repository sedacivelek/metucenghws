package project.parts.logics;

import project.SimulationRunner;
import project.components.Factory;
import project.components.Robot;
import project.parts.Arm;
import project.parts.payloads.Payload;
import project.utility.Common;

import java.util.ArrayList;
import java.util.List;

public class Fixer extends Logic
{
    @Override public void run ( Robot robot )
    {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf("Robot %02d : Fixed and waken up robot (%02d).%n", ...);
        // System.out.printf("Robot %02d : Nothing to fix, waiting!%n", ...);
        // System.out.printf("Robot %02d : Fixer woke up, going back to work.%n", ...);


        if(robot.getClass().getName().equals("project.parts.Base")){
            //retrieve serialNo with reflection
            int serialNo = (int) Common.get(robot, "serialNo");
            //retrieve broken robots list
            List<Robot> brokenRobots = SimulationRunner.factory.brokenRobots;
            List<Robot> fixedRobots = new ArrayList<>();
            //brokenrobots shared by both fixer and inspector, hence synchronized needed
            synchronized (brokenRobots){
                //if no robot to fix, wait
                if(brokenRobots.size()==0){
                    synchronized (System.out){
                        System.out.printf("Robot %02d : Nothing to fix, waiting!%n", serialNo);
                    }
                    try {
                        brokenRobots.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                }
                //fixer notified by inspector and woke up
                else{
                    synchronized (System.out){
                        System.out.printf("Robot %02d : Fixer woke up, going back to work.%n",serialNo);
                    }
                    //fetches first broken robot
                    for(Robot brokenRobot:brokenRobots){

                        boolean fixed = false;
                        //retrieve serial no of broken robot
                        int brokenSerialNo = (int) Common.get(brokenRobot,"serialNo");
                        // retrieve all parts of robot
                        Arm brokenArm = (Arm) Common.get(brokenRobot,"arm");
                        Payload brokenPayload = (Payload) Common.get(brokenRobot,"payload");
                        Logic brokenLogic = (Logic) Common.get(brokenRobot,"logic");
                        //if arm broken
                        if(brokenArm==null){
                            //set new arm
                            Common.set(brokenRobot,"arm",Factory.createPart("Arm"));
                            fixed = true;
                        }
                        //if payload broken set payload according to robot's logic
                        else if(brokenPayload==null){
                            Logic logicOfPayload = (Logic) Common.get(brokenRobot,"logic");
                            if(logicOfPayload.getClass().getName().equals("project.parts.logics.Supplier")){
                                Common.set(brokenRobot,"payload",Factory.createPart("Gripper"));
                                fixed = true;
                            }
                            else if(logicOfPayload.getClass().getName().equals("project.parts.logics.Builder")){
                                Common.set(brokenRobot,"payload",Factory.createPart("Welder"));
                                fixed = true;
                            }
                            else if(logicOfPayload.getClass().getName().equals("project.parts.logics.Inspector")){
                                Common.set(brokenRobot,"payload",Factory.createPart("Camera"));
                                fixed = true;
                            }
                            else if(logicOfPayload.getClass().getName().equals("project.parts.logics.Fixer")){
                                Common.set(brokenRobot,"payload",Factory.createPart("MaintenanceKit"));
                                fixed = true;
                            }
                        }
                        //if logic broken, set new logic according to robot's payload
                        else if(brokenLogic==null){
                            Payload payloadOfLogic = (Payload) Common.get(brokenRobot,"payload");
                            if(payloadOfLogic.getClass().getName().equals("project.parts.payloads.Gripper")){
                                Common.set(brokenRobot,"logic", Factory.createPart("Supplier"));
                                fixed = true;
                            }
                            else if(payloadOfLogic.getClass().getName().equals("project.parts.payloads.Welder")){
                                Common.set(brokenRobot,"logic",Factory.createPart("Builder"));
                                fixed = true;
                            }
                            else if(payloadOfLogic.getClass().getName().equals("project.parts.payloads.Camera")){
                                Common.set(brokenRobot,"logic",Factory.createPart("Inspector"));
                                fixed = true;
                            }
                            else if(payloadOfLogic.getClass().getName().equals("project.parts.payloads.MaintenanceKit")){
                                Common.set(brokenRobot,"logic",Factory.createPart("Fixer"));
                                fixed = true;
                            }
                        }
                        //if robot fixed, repaint robots and notify robot and break
                        if(fixed){
                            fixedRobots.add(brokenRobot);
                            synchronized (System.out){
                                System.out.printf("Robot %02d : Fixed and waken up robot (%02d).%n",serialNo,brokenSerialNo);
                            }
                            synchronized (brokenRobot){
                                brokenRobot.notify();
                            }
                            synchronized (SimulationRunner.robotsDisplay){
                                SimulationRunner.robotsDisplay.repaint();
                            }
                            break;
                        }

                    }
                    //remove working robot from broken robots
                    brokenRobots.removeAll(fixedRobots);

                }
            }




        }
    }
}