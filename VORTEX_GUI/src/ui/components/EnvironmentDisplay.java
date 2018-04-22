package ui.components;

import data.Obstacle;

import javax.swing.*;
import java.awt.*;

public class EnvironmentDisplay extends JPanel
{

    private Obstacle[] obstacles;

    public EnvironmentDisplay()
    {
        setBackground(new Color(200, 200, 200));
    }

    public void setObstacles(Obstacle[] obstacles)
    {
        this.obstacles = obstacles;
    }

    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);

        Dimension windowSize = getSize();
        int window_width = windowSize.width;
        int window_height = windowSize.height;

        draw_circle(window_width / 2, transform_y(0), 20, g);

        if(obstacles != null)
        {

            g.setColor(new Color(0, 0, 0));

            Dimension screen_extent = findScreenExtent();
            int extent_x = screen_extent.width;
            int extent_y = screen_extent.height;

            for (Obstacle o : obstacles)
            {
                int draw_x = (window_width / 2) +  (int) (( (double) o.getXComponent() / (double) extent_x) * (window_width / 2));
                int draw_y = (int) ( ( (double) o.getYComponent() / (double) extent_y ) * (window_height) );

                draw_circle(draw_x, transform_y(draw_y), 10, g);

                //Connecting line
                g.drawLine(window_width/2, transform_y(0), draw_x, transform_y(draw_y));

                //Label
                String dist = String.valueOf(o.getDist()) + " cm";
                g.drawChars(dist.toCharArray(), 0, dist.length(), (((window_width / 2) + draw_x )/ 2), transform_y(draw_y / 2) );
            }

        }
    }

    private Dimension findScreenExtent()
    {
        Dimension extent = new Dimension();

        int screen_margin = 10; //Pixels
        int extent_x = 0, extent_y = 0;

        for(Obstacle o: obstacles)
        {
            if(Math.abs(o.getXComponent()) > extent_x) extent_x = Math.abs(o.getXComponent()); //X
            if(Math.abs(o.getYComponent()) > extent_y) extent_y = Math.abs(o.getYComponent()); //Y
        }
        extent.setSize(extent_x + 2*screen_margin, extent_y + 2*screen_margin);
        return extent;
    }

    private int transform_y(int cartesian_y)
    {
        return getSize().height - cartesian_y;
    }

    private void draw_circle(int x, int y, int radius, Graphics g)
    {
        g.drawOval(x - radius, y - radius, 2*radius, 2*radius);
    }
}
