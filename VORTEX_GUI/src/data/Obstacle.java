package data;

public class Obstacle
{
    private int angle, dist, x, y;

    public Obstacle(int angle, int dist)
    {
        this.angle = angle;
        this.dist = dist;

        double angleRadians = angle * (Math.PI/180);

        this.x = (int) (dist * (Math.cos(angleRadians)));
        this.y = (int) (dist * (Math.sin(angleRadians)));
    }

    public int getAngle()
    {
        return angle;
    }

    public int getDist()
    {
        return dist;
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
