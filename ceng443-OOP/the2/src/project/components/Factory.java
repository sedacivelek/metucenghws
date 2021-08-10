package project.components;

import project.parts.Base;
import project.parts.Part;
import project.utility.Common;

import java.util.ArrayList;
import java.util.List;

public class Factory
{
    private static int nextSerialNo = 1 ;

    public static Base createBase ()
    {
        // TODO
        // This function returns a base by applying factory and abstract factory patterns

        //when new base created, increase serialNo
        nextSerialNo++;
        //Common.createBaseFactory creates new objects
        Base base = Common.createBaseFactory(nextSerialNo);
        return base;
    }

    public static Part createPart (String name )
    {
        // TODO
        // This function returns a robot part by applying factory and abstract factory patterns
        // In case the function needs to throw an exception, throw this: SmartFactoryException( "Failed: createPart!" )

        //Common.createPartFactory creates objects, if exception occurs, it throws
        Part createdPart = Common.createPartFactory(name);
        return createdPart;
    }

    public  int            maxRobots      ;
    public List<Robot>     robots         ;
    public ProductionLine  productionLine ;
    public  Storage        storage        ;
    public  List<Robot>    brokenRobots   ;
    public  boolean        stopProduction ;

    public Factory ( int maxRobots , int maxProductionLineCapacity , int maxStorageCapacity )
    {
        this.maxRobots      = maxRobots                                       ;
        this.robots         = new ArrayList<>()                               ;
        this.productionLine = new ProductionLine( maxProductionLineCapacity ) ;
        this.storage        = new Storage( maxStorageCapacity        )        ;
        this.brokenRobots   = new ArrayList<>()                               ;
        this.stopProduction = false                                           ;

        Base robot ;

        robot = createBase()                                             ;
        Common.set( robot , "arm"     , createPart( "Arm"            ) ) ;
        Common.set( robot , "payload" , createPart( "Gripper"        ) ) ;
        Common.set( robot , "logic"   , createPart( "Supplier"       ) ) ;
        robots.add(robot ) ;
        int don =(int) Common.get(robot,"serialNo");
        robot = createBase()                                             ;
        Common.set( robot , "arm"     , createPart( "Arm"            ) ) ;
        Common.set( robot , "payload" , createPart( "Welder"         ) ) ;
        Common.set( robot , "logic"   , createPart( "Builder"        ) ) ;
        robots.add(robot ) ;

        robot = createBase()                                             ;
        Common.set( robot , "arm"     , createPart( "Arm"            ) ) ;
        Common.set( robot , "payload" , createPart( "Camera"         ) ) ;
        Common.set( robot , "logic"   , createPart( "Inspector"      ) ) ;
        robots.add(robot ) ;

        robot = createBase()                                             ;
        Common.set( robot , "arm"     , createPart( "Arm"            ) ) ;
        Common.set( robot , "payload" , createPart( "Camera"         ) ) ;
        Common.set( robot , "logic"   , createPart( "Inspector"      ) ) ;
        robots.add(robot ) ;

        robot = createBase()                                             ;
        Common.set( robot , "arm"     , createPart( "Arm"            ) ) ;
        Common.set( robot , "payload" , createPart( "MaintenanceKit" ) ) ;
        Common.set( robot , "logic"   , createPart( "Fixer"          ) ) ;
        robots.add(robot ) ;

        robot = createBase()                                             ;
        Common.set( robot , "arm"     , createPart( "Arm"            ) ) ;
        Common.set( robot , "payload" , createPart( "MaintenanceKit" ) ) ;
        Common.set( robot , "logic"   , createPart( "Fixer"          ) ) ;
        robots.add(robot ) ;
    }

    public void start ()
    {
        for ( Robot r : robots )  { new Thread( r ).start() ; }
    }

    public void initiateStop ()
    {
        stopProduction = true ;

        synchronized ( robots )
        {
            for ( Robot r : robots )  { synchronized ( r )  { r.notifyAll() ; } }
        }

        synchronized ( productionLine )  { productionLine.notifyAll() ; }
        synchronized ( brokenRobots   )  { brokenRobots  .notifyAll() ; }
    }
}