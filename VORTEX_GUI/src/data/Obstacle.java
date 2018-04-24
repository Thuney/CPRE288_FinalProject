package data;

public class Obstacle
{
    private int angle, dist_cm, x, y;

    public Obstacle(int angle, int dist_cm)
    {
        this.angle = angle;
        this.dist_cm = dist_cm;

        double angleRadians = getAngleRadians();

        this.x = (int) (dist_cm * (Math.cos(angleRadians)));
        this.y = (int) (dist_cm * (Math.sin(angleRadians)));
    }

    public int getAngle()
    {
        return angle;
    }

    public double getAngleRadians()
    {
        return ((double) (this.angle * (Math.PI/180)));
    }

    public int getDist()
    {
        return dist_cm;
    }

    public int getXComponent()
    {
        return x;
    }

    public int getYComponent()
    {
        return y;
    }
}
