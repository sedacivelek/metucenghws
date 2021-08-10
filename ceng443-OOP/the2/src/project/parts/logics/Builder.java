package project.parts.logics;

import project.SimulationRunner;
import project.components.Robot;
import project.parts.Arm;
import project.parts.Part;
import project.parts.logics.Logic;
import project.parts.payloads.Payload;
import project.utility.Common;

import java.util.ArrayList;
import java.util.List;

public class Builder extends Logic {
    @Override
    public void run(Robot robot) {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf("Robot %02d : Builder cannot build anything, waiting!%n", ...);
        // System.out.printf("Robot %02d : Builder woke up, going back to work.%n", ...);
        // System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", ...);

        //isBuilt set true if builder finds a part to attach
        boolean isBuilt = false;
        if (robot.getClass().getName().equals("project.parts.Base")) {
            //retrieve serialNo with reflection
            int serialNo = (int) Common.get(robot, "serialNo");
            //production line list
            List<Part> parts = SimulationRunner.factory.productionLine.parts;
            //collect attached part that will be removed from production line
            List<Part> partsChanged = new ArrayList<>();
            //parts list is shared from both builder and supplier, hence synronized needed
            synchronized (parts) {
                //if there is part in production line, wait
                synchronized (System.out) {
                    System.out.printf("Robot %02d : Builder cannot build anything, waiting!%n", serialNo);
                }

                try {
                    parts.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                //builder notified by supplier and woke up
                synchronized (System.out) {
                    System.out.printf("Robot %02d : Builder woke up, going back to work.%n", serialNo);
                }

                for (Part changePart : parts) {
                    Class changePartClass = changePart.getClass();
                    if (changePartClass.getName().equals("project.parts.Base")) {
                        Arm arm = (Arm) Common.get(changePart, "arm");
                        Payload payload = (Payload) Common.get(changePart, "payload");
                        Logic logic = (Logic) Common.get(changePart, "logic");
                        //if base doesn't have arm
                        if (arm == null) {
                            //search for arm
                            for (Part armPart : parts) {
                                boolean isArm = armPart.getClass().getName().equals("project.parts.Arm");
                                //if arm is found
                                if (isArm) {
                                    //set arm field of base with founded arm
                                    Common.set(changePart, "arm", armPart);
                                    partsChanged.add(armPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                            }
                        }
                        //if base+arm doesnt have a payload
                        else if (payload == null) {
                            //search for payload
                            for (Part payloadPart : parts) {
                                // if found gripper
                                if (payloadPart.getClass().getName().equals("project.parts.payloads.Gripper")) {
                                    //set payload field of base
                                    Common.set(changePart, "payload", payloadPart);
                                    partsChanged.add(payloadPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                                //if found welder
                                else if (payloadPart.getClass().getName().equals("project.parts.payloads.Welder")) {
                                    //set payload field of base
                                    Common.set(changePart, "payload", payloadPart);
                                    partsChanged.add(payloadPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                                //if found camera
                                else if (payloadPart.getClass().getName().equals("project.parts.payloads.Camera")) {
                                    //set payload field of base
                                    Common.set(changePart, "payload", payloadPart);
                                    partsChanged.add(payloadPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                                //if found maintenance kit
                                else if (payloadPart.getClass().getName().equals("project.parts.payloads.MaintenanceKit")) {
                                    //set payload of base
                                    Common.set(changePart, "payload", payloadPart);
                                    partsChanged.add(payloadPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                            }
                        }
                        //if base+arm+payload doesnt have a logic unit
                        else if (logic == null) {
                            //search for logic
                            for (Part logicPart : parts) {
                                //if payload is gripper and found supplier
                                if (logicPart.getClass().getName().equals("project.parts.logics.Supplier") && payload.getClass().getName().equals("project.parts.payloads.Gripper")) {
                                    //set logic field of base
                                    Common.set(changePart, "logic", logicPart);
                                    partsChanged.add(logicPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                                //if payload is welder and found builder
                                else if (logicPart.getClass().getName().equals("project.parts.logics.Builder") && payload.getClass().getName().equals("project.parts.payloads.Welder")) {
                                    //set logic field of base
                                    Common.set(changePart, "logic", logicPart);
                                    partsChanged.add(logicPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                                //if payload is camera and found inspector
                                else if (logicPart.getClass().getName().equals("project.parts.logics.Inspector") && payload.getClass().getName().equals("project.parts.payloads.Camera")) {
                                    //set logic field of base
                                    Common.set(changePart, "logic", logicPart);
                                    partsChanged.add(logicPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                                //if payload is maintenance kit and found fixer
                                else if (logicPart.getClass().getName().equals("project.parts.logics.Fixer") && payload.getClass().getName().equals("project.parts.payloads.MaintenanceKit")) {
                                    //set logic field of base
                                    Common.set(changePart, "logic", logicPart);
                                    partsChanged.add(logicPart);
                                    isBuilt = true;
                                    synchronized (System.out) {
                                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                    }
                                    break;
                                }
                            }
                        }
                        //if robot is completed, add to robot and start a new robot thread to speed up production
                        else if (arm != null && logic != null && payload != null && logic != null) {
                            if (SimulationRunner.factory.robots.size() < SimulationRunner.factory.maxRobots) {
                                synchronized (SimulationRunner.factory.robots) {
                                    Robot robot1 = (Robot) changePart;
                                    SimulationRunner.factory.robots.add(robot1);
                                    new Thread(robot1).start();

                                }
                                partsChanged.add(changePart);
                                isBuilt = true;
                            }
                            //if robot list is full, add robot to storage
                            else {
                                List<Robot> storage = (List<Robot>) Common.get(SimulationRunner.factory.storage, "robots");
                                int maxStorage = (int) Common.get(SimulationRunner.factory.storage, "maxCapacity");
                                if (storage.size() < maxStorage) {
                                    synchronized (storage) {
                                        storage.add((Robot) changePart);
                                    }
                                    partsChanged.add(changePart);
                                    isBuilt = true;
                                }
                                //if storage is full, stop production
                                if(storage.size()==maxStorage) {
                                    SimulationRunner.factory.initiateStop();
                                }
                            }

                        }

                    }
                    if (isBuilt) break;

                }
                //remove attached part from production line
                if (isBuilt) {
                    for (Part cp : partsChanged) {
                            parts.remove(cp);
                    }
                }

            }

            //repaint
            synchronized (SimulationRunner.productionLineDisplay) {
                SimulationRunner.productionLineDisplay.repaint();
            }
            synchronized (SimulationRunner.robotsDisplay) {
                SimulationRunner.robotsDisplay.repaint();
            }
            synchronized (SimulationRunner.storageDisplay) {
                SimulationRunner.storageDisplay.repaint();
            }

        }

    }
}