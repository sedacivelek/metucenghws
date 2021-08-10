package project.parts.logics;

import project.SimulationRunner;
import project.components.Factory;
import project.components.Robot;
import project.parts.Part;
import project.utility.Common;

import java.util.ArrayList;
import java.util.List;

public class Supplier extends Logic
{
    @Override public void run ( Robot robot )
    {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf( "Robot %02d : Supplying a random part on production line.%n", ...);
        // System.out.printf( "Robot %02d : Production line is full, removing a random part from production line.%n", ...);
        // System.out.printf( "Robot %02d : Waking up waiting builders.%n", ...);

        Class base = robot.getClass();
        if(base.getName().equals("project.parts.Base")){
            //retrieve serialNo with reflection
            int serialNo = (int) Common.get(robot, "serialNo");
            //production line list
            List<Part> parts = SimulationRunner.factory.productionLine.parts;
            //max capacity of production line
            int maxCap = SimulationRunner.factory.productionLine.maxCapacity;

            //Randomly choose a part to create
            int randomInt = Common.random.nextInt(10);
            String createPartName = "";
            switch (randomInt){
                case 0:
                    createPartName = "project.parts.Base";
                    break;
                case 1:
                    createPartName = "Arm";
                    break;
                case 2:
                    createPartName = "Supplier";
                    break;
                case 3:
                    createPartName = "Gripper";
                    break;
                case 4:
                    createPartName = "Builder";
                    break;
                case 5:
                    createPartName = "Welder";
                    break;
                case 6:
                    createPartName = "Inspector";
                    break;
                case 7:
                    createPartName = "Camera";
                    break;
                case 8:
                    createPartName = "Fixer";
                    break;
                case 9:
                    createPartName = "MaintenanceKit";
                    break;
            }
            synchronized (parts){
            //if randomly chosen part is Base
            if(createPartName.equals("project.parts.Base")){
                //if production line is full
                if(parts.size()==maxCap){
                    //remove random part from production line and notify
                    parts.remove(Common.random.nextInt(maxCap));
                    synchronized (System.out){
                        System.out.printf( "Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                        System.out.printf( "Robot %02d : Waking up waiting builders.%n",serialNo);
                    }
                    synchronized (SimulationRunner.productionLineDisplay){
                        SimulationRunner.productionLineDisplay.repaint();
                    }

                        parts.notifyAll();

                }
                //if production line is not full
                else if(parts.size()<maxCap){
                    //create Base, add to production line and notify
                    parts.add(Factory.createBase());
                    synchronized (System.out){
                        System.out.printf( "Robot %02d : Supplying a random part on production line.%n", serialNo);
                        System.out.printf( "Robot %02d : Waking up waiting builders.%n",serialNo);
                    }
                    synchronized (SimulationRunner.productionLineDisplay){
                        SimulationRunner.productionLineDisplay.repaint();
                    }

                        parts.notifyAll();

                }
            }
            //if randomly chosen part is not Base
            else {
                //if production line is full
                if (parts.size() == maxCap) {
                    //remove random part from production line and notify
                    parts.remove(Common.random.nextInt(maxCap));
                    synchronized (System.out) {
                        System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                        System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                    }
                    synchronized (SimulationRunner.productionLineDisplay) {
                        SimulationRunner.productionLineDisplay.repaint();
                    }

                        parts.notifyAll();

                }
                //if production line is not empty
                else if (parts.size() < maxCap) {
                    //create part, add to production line and notify
                    parts.add(Factory.createPart(createPartName));
                    synchronized (System.out) {
                        System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                        System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                    }
                    synchronized (SimulationRunner.productionLineDisplay) {
                        SimulationRunner.productionLineDisplay.repaint();
                    }

                        parts.notifyAll();

                }
            }
            }




        }




    }
}