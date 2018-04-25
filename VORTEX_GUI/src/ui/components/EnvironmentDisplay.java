package ui.components;

import data.Obstacle;

import javax.swing.*;
import java.awt.*;

public class EnvironmentDisplay extends JPanel
{

    final static double cybot_radius_cm = 18;
    final static double sensor_radius_cm = 14.5;
    final static double thin_obstacle_radius_cm = 3;
    final static double thick_obstacle_radius_cm = 5.75;

    final static double sensor_module_radius_cm = 14.5;

    final static int testRadius = 10;

    private double screen_extent_cm;

    private Obstacle[] obstacles;

    public EnvironmentDisplay()
    {
        setBackground(new Color(200, 200, 200));
    }

    /**
     * Set the reference to obstacles to be drawn
     * @param obstacles
     */
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

        if(obstacles != null)
        {

            g.setColor(new Color(0, 0, 0));

            int margin = 10; //cm
            this.screen_extent_cm = findMaxDist() + margin + cybot_radius_cm;

            //Draw CyBot
            draw_circle(window_width / 2, transform_y(0), centimeters_to_pixels(cybot_radius_cm), new Dimension(0, 0), g);

            //Draw obstacles
            for (Obstacle o : obstacles)
            {
                int sensor_offset_x = centimeters_to_pixels( sensor_module_radius_cm*Math.cos(o.getAngleRadians()) );
                int sensor_offset_y = centimeters_to_pixels( sensor_module_radius_cm*Math.sin(o.getAngleRadians()) );

                int draw_x = ((window_width / 2) +  centimeters_to_pixels(o.getXComponent())) + sensor_offset_x;
                int draw_y = centimeters_to_pixels(o.getYComponent()) + sensor_offset_y;

                Dimension offset = findDrawOffset(o, centimeters_to_pixels(thick_obstacle_radius_cm));

                draw_circle(draw_x, transform_y(draw_y), centimeters_to_pixels(thick_obstacle_radius_cm), offset, g);

                //Connecting line
                g.drawLine(window_width/2 + sensor_offset_x, transform_y(0 + sensor_offset_y), draw_x, transform_y(draw_y));

                //Label
                String dist = String.valueOf(o.getDist()) + " cm";
                g.drawChars(dist.toCharArray(), 0, dist.length(), (((window_width / 2) + draw_x )/ 2), transform_y(draw_y / 2) );
            }

        }
    }

    /**
     * Convert a measurement in centimeters to pixels
     * @param centimeters
     * @return Pixel measurement
     */
    private int centimeters_to_pixels(double centimeters)
    {
        return ((int) ( ( (centimeters) / (screen_extent_cm) )*(getSize().width/2)));
    }

    /**
     * Convert a pixel measurement to centimeters
     * @param pixels
     * @return Measurement in centimeters
     */
    private int pixels_to_centimeters(int pixels)
    {
        return ((int) ((((double) pixels) / ((double) (getSize().width/2))) * screen_extent_cm));
    }

    /**
     * Finds max dimension in the x or y direction
     * (Used for scaling x-axis to always include detected objects)
     * @return
     */
    private int findMaxDist()
    {
        int extent = 0;

        for(Obstacle o: obstacles)
        {
            if(Math.abs(o.getXComponent()) > extent) extent = Math.abs(o.getXComponent()); //X
            if(Math.abs(o.getYComponent()) > extent) extent = Math.abs(o.getYComponent()); //Y
        }

        return extent;
    }

    /**
     * Calculates the offset to the center for circles tangent to a line at an angle
     * (Necessary for good approximation of Obstacle positioning)
     * @param o
     * @param radius_px
     * @return a Dimension object denoting the width and height of offset from the center to the provided point
     */
    private Dimension findDrawOffset(Obstacle o, int radius_px)
    {
        Dimension offset = new Dimension();

        double angleRadians = o.getAngleRadians();

        double alpha = (Math.PI/2) - angleRadians;

        int offset_center_x = (int) (-(radius_px*Math.cos(alpha)));
        int offset_center_y = (int) (-(radius_px*Math.sin(alpha)));

        offset.setSize(offset_center_x, offset_center_y);

        return offset;
    }

    /**
     * Flips the y coordinate to match the screen coordinates
     * @param cartesian_y
     * @return
     */
    private int transform_y(int cartesian_y)
    {
        return getSize().height - cartesian_y;
    }

    /**
     * Draws a circle centered at its center (Swing Graphics.drawOval() does not do this), and offset by the provided dimension
     * @param x
     * @param y
     * @param radius
     * @param offset
     * @param g
     */
    private void draw_circle(int x, int y, int radius, Dimension offset, Graphics g)
    {
        g.drawOval(x - radius + offset.width, y - radius + offset.height, 2*radius, 2*radius);
    }
 }
