import java.util.PriorityQueue;
import java.util.Random;


public class BetterNode implements Comparable<BetterNode> {
        private static final double BACKOFF_CONSTANT = 51.2; // μseconds! BackOff Constant is the constant
	private Double time; //TIME TO SEND
	private Double lambda;
	private int collisionCount; // Number of collisions
        private boolean initilized = false;
	//new node has no collisions
	private boolean collided;
	private Random ran;
	private PriorityQueue<Packet> packetQueue;


	/**
	 * Initializes a new Node at time t
	 */
	public BetterNode(double aLambda)
	{
                collisionCount = 0;
		lambda = aLambda;
		time = poisson(aLambda);
		collided = false;
		packetQueue = new PriorityQueue<Packet>();
	}

	/**
	 * This gives our random variable
	 * @param lambda the lambda value
	 * @return random variable
	 */
	public double poisson(double aLambda)
	{
		double u;
		double val;

		u = Math.random();  

		val = (-1) * Math.log(u) * aLambda; // x= -λ log u
		return val;
	}

	/**
	 * Adds Poisson X value to current time
	 */
	public void send(double aLambda)
	{
		collisionCount=0; // Resets the number of collisions. Used for backoff()
                if(initilized)
                {
                    time += poisson(aLambda); //The normal behavior of send
                }
                else
                    initilized=!initilized; // Special condition for the first time it is run
	}


	@Override
	public int compareTo(BetterNode o)
	{
		return this.time.compareTo(o.time);
	}

	public double getLambda()
	{
		return lambda;
	}

	public double getTime()
	{
		return time;
	}
        
        
     /**
     * Method to be run if a collision is detected
     */
    public void collide()
    {
        time += 1 + backoff(); // our next attempt
        System.out.println("collision count " + collisionCount);
    }
    
	/**
	 * Checks to see if the node collided
	 * @return true if collided, false otherwise
	 */
	public boolean isCollided()
	{
		//returns the status of the node if it is collided or not
		return collided;
	}

	/**
	 * Sets the boolean value of collided to true if collided
	 */
	public void didCollided()
	{
		collided = true;
	}

	/**
	 * Sets the boolean value of collided to false
	 */
	public void notCollided()
	{
		collided = false;
	}
        
	@Override
	public String toString()
	{
		return "Start: "+ time + "\n"; // Formats the String a little more neatly
	}

	public double backoff() // version of backoff that does not rely on an external parameter
	{
		collisionCount++; // number of collisions seen so far
		ran = new Random(collisionCount);
		double delay = ((ran.nextInt(2))*(Math.pow(2, collisionCount) - 1)); // ran.nextInt(2) gives an int between 0 and 2 (exclusive)
                                                                                     // which randomly decides if time is incremented
		return delay; // * BACKOFF_CONSTANT;
	}

	public double backoff(int numberCollision) 
	{
		int n = numberCollision; // number of collisions seen so far
		ran = new Random(n);
		int min = 0;
		int delay = 0;
		delay = ((int) ((Math.pow(2, n)) - 1)) + ran.nextInt();

		return delay; // * BACKOFF_CONSTANT;
	}

	public Packet createPacket(int i, double time) 
	{
		return new Packet(i, time);
	}

	public void addPacketsToQueue(Packet aPacket) 
	{
		packetQueue.add(aPacket);
	}

	public Packet getNextPacketToBeTransmitted() 
	{
		return packetQueue.poll();
	}


}