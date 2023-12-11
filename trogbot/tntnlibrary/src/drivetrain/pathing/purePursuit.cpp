#include "../tntnlibrary/include/drivetrain/pathing/purePursuit.h"
#include "../tntnlibrary/include/util.h"

using namespace tntnlib;

PurePursuit purePursuit;

float PurePursuit::min(float first, float second)
{
    if (first < second)
    {
        return first;
    }
    else
    {
        return second;
    }
}

float PurePursuit::max(float first, float second)
{
    if (first > second)
    {
        return first;
    }
    else
    {
        return second;
    }
}

float PurePursuit::hypotenuse(float x1, float y1, float x2, float y2)
{ // finds the length of a line drawn between two points
    float X = x1 - x2;
    float Y = y1 - y2;
    float dist = sqrt(X * X + Y * Y);
    return dist;
}

void PurePursuit::searchForIntersect(Path &path, Pose &pose, float lookAhead)
{
    // ALGORITHM CREDIT GOES TO: BLRS wiki and Sarah Xiang from VRC team 97963A and VEXU team ILLINI
    // output (intersections found) should be stored in arrays sol1 and sol2 in the form of sol1 = [sol1_x, sol1_y]
    // if two solutions are the same, store the same values in both sol1 and sol2
    /*
        if discriminant < 0 : no intersection
        if discriminant = 0 : tangent
        if discriminant > 0 : intersection
    */
    float sol1_x{0};
    float sol1_y{0};
    float sol2_x{0};
    float sol2_y{0};
    // goal_x = 0, goal_y = 0;

    // boolean variable to keep track of if intersections are found
    bool intersectFound = false;
    float currentX = pose.x;
    float currentY = pose.y;
    starting_index = last_found_index;
    for (int i = starting_index; i < path.numberOfPoints - 1; i++)
    {
        // beginning of line-circle intersection code
        // subtract currentX and currentY from [x1, y1] and [x2, y2] to offset the system to origin
        float x1 = path.x[i] - currentX;
        float y1 = path.y[i] - currentY;
        float x2 = path.x[i + 1] - currentX;
        float y2 = path.y[i + 1] - currentY;
        // printf("(%.2f,%.2f),", x1, y1);
        // printf("(%.2f,%.2f)\n", x2, y2);

        // printf("(%.2f,%.2f),(%.2f,%.2f),(%.2f,%.2f),(%.2f,%.2f)\n", x1, x2, y1, y2, x1_offset, x2_offset, y1_offset, y2_offset);
        //  output (intersections found) should be stored in arrays sol1 and sol2 in the form of sol1 = [sol1_x, sol1_y]
        //  if two solutions are the same, store the same values in both sol1 and sol2
        float r = lookAhead;
        float dx = x2 - x1;
        float dy = y2 - y1;
        // printf("(%.2f,%.2f)\n", dx,dy);
        float dr = sqrt(dx * dx + dy * dy);
        // printf("%.2f\n", dr);
        float D = x1 * y2 - x2 * y1;
        float discriminant = (r * r) * (dr * dr) - (D * D);

        if (discriminant >= 0)
        {
            float rootD = sqrt(discriminant);
            // calculate the solutions
            sol1_x = (D * dy + sgn(dy) * dx * rootD) / (dr * dr);
            sol2_x = (D * dy - sgn(dy) * dx * rootD) / (dr * dr);
            sol1_y = (-D * dx + fabs(dy) * rootD) / (dr * dr);
            sol2_y = (-D * dx - fabs(dy) * rootD) / (dr * dr);

            // add currentX and currentY back to the solutions, offset the system back to its original position
            sol1_x += currentX;
            sol1_y += currentY;
            sol2_x += currentX;
            sol2_y += currentY;
            // end of line-circle intersection code

            // find min and max x y values
            float minX = min(path.x[i], path.x[i + 1]);
            float maxX = max(path.x[i], path.x[i + 1]);
            float minY = min(path.y[i], path.y[i + 1]);
            float maxY = max(path.y[i], path.y[i + 1]);

            // check to see if any of the two solution points are within the correct range
            // for a solution point to be considered valid, its x value needs to be within minX and maxX AND its y value needs to be between minY and maxY
            // if sol1 OR sol2 are within the range, intersection is found

            bool sol1_x_domain, sol1_y_range, sol2_x_domain, sol2_y_range;
            if (minX <= sol1_x && sol1_x <= maxX)
                sol1_x_domain = true;
            else
                sol1_x_domain = false;
            if (minX <= sol2_x && sol2_x <= maxX)
                sol2_x_domain = true;
            else
                sol2_x_domain = false;
            if (minY <= sol1_y && sol1_y <= maxY)
                sol1_y_range = true;
            else
                sol1_y_range = false;
            if (minY <= sol2_y && sol2_y <= maxY)
                sol2_y_range = true;
            else
                sol2_y_range = false;

            bool sol1, sol2;
            if (sol1_x_domain && sol1_y_range)
                sol1 = true;
            else
                sol1 = false;
            if (sol2_x_domain && sol2_y_range)
                sol2 = true;
            else
                sol2 = false;

            if (sol1 || sol2)
            {
                intersectFound = true;

                // if both solutions are in range, check which one is better
                if (sol1 && sol2)
                {
                    // make the decision by comparing the distance between the intersections and the next point in path
                    if (hypotenuse(sol1_x, sol1_y, path.x[i + 1], path.y[i + 1]) < hypotenuse(sol2_x, sol2_y, path.x[i + 1], path.y[i + 1]))
                    {
                        // printf("sol1 better\n");
                        tempTarget.x = sol1_x, tempTarget.y = sol1_y;
                    }
                    else
                    {
                        // printf("sol2 better\n");
                        tempTarget.x = sol2_x, tempTarget.y = sol2_y;
                    }
                }
                // if not both solutions are in range, take the one that's in range
                else
                {
                    // if solution pt1 is in range, set that as goal point
                    if (sol1)
                    {
                        // printf("just sol1\n");
                        tempTarget.x = sol1_x, tempTarget.y = sol1_y;
                    }
                    else
                    {
                        // printf("just sol2\n");
                        tempTarget.x = sol2_x, tempTarget.y = sol2_y;
                    }
                }

                // only exit loop if the solution pt found is closer to the next pt in path than the current pos
                if (hypotenuse(tempTarget.x, tempTarget.y, path.x[i + 1], path.y[i + 1]) < hypotenuse(currentX, currentY, path.x[i + 1], path.y[i + 1]))
                {
                    // printf("sol better\n");
                    //   update lastFoundIndex and exit
                    target.x = tempTarget.x;
                    target.y = tempTarget.y;
                    last_found_index = i;
                    // printf("(%.2f,%.2f)\n", path1.x[last_found_index], path1.y[last_found_index]);
                    // printf("(%.2f,%.2f)\n", path1.x[last_found_index + 1], path1.y[last_found_index + 1]);

                    break;
                }
                else
                {
                    // in case for some reason the robot cannot find intersection in the next path segment, but we also don't want it to go backward
                    // printf("no intersect\n");
                    last_found_index = i + 1;
                    tempTarget.x = path.x[last_found_index];
                    tempTarget.y = path.y[last_found_index];
                }
            }
            // if no solutions are in range
        }
        else
        {
            intersectFound = false;
            // no new intersection found, potentially deviated from the path
            // follow path[lastFoundIndex]
            tempTarget.x = path.x[last_found_index], tempTarget.y = path.y[last_found_index];
        }
        // std::cout << std::flush;
    }
    // printf("(%.2f,%.2f)\n", goal_x, goal_y);
}

void PurePursuit::getNextIntersect(Path &path, Pose &pose, float lookAhead)
{
    Pose pathEnd(path.x3, path.y3, 0);
    if (pose.distance(pathEnd) > lookAhead)
    {
        searchForIntersect(path, pose, lookAhead);
        target.x = tempTarget.x, target.y = tempTarget.y;
    }
    else
    {
        last_found_index = path.numberOfPoints;
        target.x = path.x3, target.y = path.y3;
    }
}