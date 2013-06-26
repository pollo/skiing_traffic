/*
 *
 * Skier methods definition
 *
 */

using namespace std;

#include "skier.h"
#include "parameters.h"
#include "physicalforces.h"
#include "slope.h"
#include <set>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>

#define EPS 0.00001
#define K 0.00001

//haven't yet find a good reason for the existance of this constructor
Skier::Skier(int id,
             const Slope& slope,
             const Vector &position,
             const Vector &direction,
             const Vector &velocity,
             const Vector &acceleration,
             double mass,
             double turning_radius,
             double skidding_factor):
  id(id), slope(slope), position(position), direction(direction),
  velocity(velocity), acceleration(acceleration),
  mass(mass), meters_since_last_waypoint(0),
  turning_radius(turning_radius), skidding_factor(skidding_factor)
{
  //the skier is not allowed to fly
  (this->position.z) = get_current_elevation();
  //TODO: check that direction lies on the slope plane
  //TODO: check that velocity lies on the slope plane
  //TODO: check that acceleration lies on the slope plane
}

Skier::Skier(int id,
             const Slope& sl,
             const Point &position,
             double mass,
             double turning_radius,
             double skidding_factor) :
  id(id), slope(sl), position(position), mass(mass),
  turning_right(false), turning_left(false), meters_since_last_waypoint(0),
  turning_radius(turning_radius), skidding_factor(skidding_factor)
{
  double slope, aspect;
  slope = get_current_slope();
  aspect = get_current_aspect();
  //the skier is not allowed to fly
  (this->position).z = get_current_elevation();
  //initial direction is along the fall line
  direction.x = cos(slope) * cos(aspect);
  direction.y = cos(slope) * sin(aspect);
  direction.z = sin(slope) * -1;
  assert(abs(-slope - get_current_inclination_angle()) < EPS);
  //initial velocity is settings::initial_vel (equal to direction)
  velocity = direction * settings::initial_vel;
  //initial acceleration is 0
  acceleration.x = 0;
  acceleration.y = 0;
  acceleration.z = 0;
}

//returns the inclination angle along the direction
double Skier::get_current_inclination_angle() const
{
  return slope.get_slope_from_p1_to_p2(position,position+direction);
}

void Skier::decide_turn()
{
  Vector force(0,0,0);
  Point a;
  double alfa, dir;
  double delta = settings::directional_deviation;
  const set<SocialForce*>& forces = slope.get_social_forces();
  for (set<SocialForce*>::const_iterator iter = forces.begin();
       iter != forces.end(); ++iter)
  {
    force += (*iter)->apply(*this);
  }
  alfa = force.angle_on_xyplane();
  dir = direction.angle_on_xyplane();
  if (abs(alfa-dir) > delta)
  {
    if (alfa < M_PI)
    {
      if (dir - alfa > 0 && dir-alfa < M_PI)
      {
        //turn right
        turning_right = true;
        turning_left = false;
      }
      else
      {
        //turn left
        turning_right = false;
        turning_left = true;
      }
    }
    else
    {
      if (alfa -dir > 0 && alfa -dir < M_PI)
      {
        //turn right
        turning_right = false;
        turning_left = true;
      }
      else
      {
        //turn left
        turning_right = true;
        turning_left = false;
      }
    }
  }
  if (abs(alfa-dir) < settings::stop_turning)
  {
    turning_right = false;
    turning_left = false;
  }
}

void Skier::choose_waypoint(double dtime)
{
  bool waypoint_valid;
  double dist_left, dist_right;
  Point left, right;
  Vector line_left, line_right, line_midle, line_waypoint;
  double alfa;
  double angle, angle_waypoint;
  //computes distances and angles
  dist_left = current_distance_from_left(&left);
  dist_right = current_distance_from_right(&right);
  line_left = left - position;
  line_left.z = 0;
  line_left.normalize();
  line_right = right - position;
  line_right.z = 0;
  line_right.normalize();
  line_waypoint = waypoint - position;
  line_waypoint.z = 0;
  line_waypoint.normalize();
  //if the skier is too near the edge avoid to choose waypoint that can make
  //the skier collide with the ege
  if (dist_left < settings::limit_edge_distance)
  {
    //narrow left is the angle under which waypoints should not be considered
    double narrow_left = acos(dist_left / settings::limit_edge_distance);
    line_left.rotate(-narrow_left);
  }
  if (dist_right < settings::limit_edge_distance)
  {
    //narrow right is the angle under which waypoints should not be considered
    double narrow_right = acos(dist_right / settings::limit_edge_distance);
    line_right.rotate(narrow_right);
  }
  //compute alfa angle between right line and left_line
  double angle_right = line_right.angle_on_xyplane();
  double angle_left = line_left.angle_on_xyplane();
  if (angle_right < angle_left)
  {
    alfa = angle_left - angle_right;
  }
  else
  {
    alfa = 2*M_PI - (angle_right - angle_left);
  }

  //compute line between the left bound and the right bound
  line_midle = line_right;
  line_midle.rotate(alfa/2.0);

  //the old waypoint is valid if the angle between the midle line
  //and the waypoint line is less then alfa/2.0
  angle_waypoint = acos(line_midle.x * line_waypoint.x +
                        line_midle.y * line_waypoint.y);

  waypoint_valid = (angle_waypoint < alfa/2.0
                    && slope.is_inside_slope(waypoint));

  if (meters_since_last_waypoint > settings::space_between_waypoints ||
      !waypoint_valid)
  {
    //narrow the acceptable angle
    alfa *= velocity.norm() / settings::vel_high;

    Vector check_slope = position+line_midle*settings::distance_to_check_slope;
    if (slope.is_inside_slope(check_slope))
    {
      double next_slope = slope.get_slope(check_slope);
      //if in the next meters there there is a small slope
      if (next_slope < settings::small_slope)
      {
        //narrow the acceptable angle to increase the speed
        alfa *= next_slope/settings::small_slope;
      }
    }

    //choose a random angle for the waypoint
    angle = (double)rand()/(double)RAND_MAX * alfa;
    angle -= alfa/2.0;

    line_midle *= settings::distance_waypoint;
    line_midle.rotate(angle);
    waypoint = position + line_midle;
    waypoint.z = slope.get_elevation(waypoint);
    meters_since_last_waypoint = 0;
  }
}

//Updates the position and the parameters of the skier for a time of dtime
//The acceleration that acted during this dtime is compute (start position
//is considered).
//The position of the skier is update moving the skier according to the
//velocity computed in the previous step.
//The new velocity is computed according to the old velocity and the effect
//of the acceleration. Finally the new direction is derived by the velocity.
void Skier::update(double dtime)
{
  bool cell_border_reached = false;
  double time_to_reach_border;
  //moves skier for dtime or until the cell border is reached
  cell_border_reached = update_position(dtime, &time_to_reach_border);
  if (cell_border_reached)
  {
    //adjust direction to have inclination angle of the plane
    //because it will be used in the acceleration computation
    direction.rotate_axis_xy(direction.inclination_angle() -
                             get_current_inclination_angle());
  }
  update_acceleration();
  //if the cell border is reached update velocity only for the time used
  if (cell_border_reached)
    update_velocity(time_to_reach_border, true);
  else
    update_velocity(dtime, false);
  update_direction();
  if (velocity.norm() > 0 )
    assert(abs(velocity.inclination_angle()-get_current_inclination_angle())<EPS);

  if (cell_border_reached)
    update(dtime-time_to_reach_border);
  choose_waypoint(dtime);
  //start turning if needed
  decide_turn();
}

void Skier::reflect_edge(const Vector d)
{
    Point intersection;
    Point difference;
    double angle;
    slope.reflect_line(position,d,&intersection,&angle);
    //vector from position to intersection
    difference = position - intersection;
    //vector from intersection to destination
    difference.rotate(2*angle);
    position = intersection + difference;
    position.z = get_current_elevation();
    //update velocity and direction
    direction.rotate(2*angle);
    velocity.x *= -1;
    velocity.y *= -1;
    velocity.rotate(2*angle);
    //update x and y of direction in order to compute inclination angle
    update_direction();
    velocity.rotate_axis_xy(velocity.inclination_angle() -
                            get_current_inclination_angle());
    update_direction();
    assert(slope.is_inside_slope(position));
}

//Moves the skier for dtime according to the velocity.
//Returns wheter the skier reached the border of the cell or not
//If the border is reached the time used is stored in time_to_reach_border
//If the slope edge is reached the skier is "reflected"
bool Skier::update_position(double dtime, double* time_to_reach_border)
{
  bool border_reached;
  Point d;
  double east, west, north, sud;
  slope.get_cell_bound(position,&east,&west,&north,&sud);
  d = position + velocity * dtime;
  //if new position is inside the cell
  if ((d.x>=west && d.x<east) && (d.y>=sud && d.y<north))
  {
    double elev = slope.get_elevation(d);
    cout.precision(10);
    slope.get_elevation(d);
    slope.get_elevation(position);
    assert(abs(d.z - elev) < EPS);
    d.z = elev;
    border_reached = false;
  }
  else
  {
    double xtime, ytime, time;
    double elev;
    //time to reach x border
    if (velocity.x > 0)
      xtime = (east - position.x) / velocity.x;
    else
      xtime = (west - position.x) / velocity.x;
    //time to reach y border
    if (velocity.y > 0)
      ytime = (north - position.y) / velocity.y;
    else
      ytime = (sud - position.y) / velocity.y;
    time = min(xtime,ytime);
    d = position + velocity * time;
    //adding K needed to cross the cell
    if (xtime<ytime)
      d.x += K * (velocity.x > 0 ? 1 : -1);
    else
      d.y += K * (velocity.y > 0 ? 1 : -1);
    *time_to_reach_border = time;
    border_reached = true;
    elev = slope.get_elevation(d);
    //check that height difference from one cell to another is not
    //greater than 20 cm
    if (abs(d.z - elev) > 0.3)
    {
      static int i=1;
      cout.precision(13);
      cout << "Warning: From position x=" << position.x <<" y="<<position.y;
      cout <<"to x="<<d.x<<" y="<<d.y<<" there is a difference in elevation";
      cout <<" estimated and real elevation of "<<abs(d.z - elev)<<endl;
      i++;
    }
    d.z = elev;
  }
  meters_since_last_waypoint += (d-position).norm();
  if (!slope.is_inside_slope(d))
  {
    reflect_edge(d);
  }
  else
    position = d;
  return border_reached;
}

void Skier::update_acceleration()
{
  Vector force(0,0,0);
  const set<PhysicalForce*>& forces = slope.get_physical_forces();
  for (set<PhysicalForce*>::const_iterator iter = forces.begin();
       iter != forces.end(); ++iter)
  {
    //force += (*iter)->apply(*this);
    Vector tmp =(*iter)->apply(*this);
    if (tmp.norm() > 0)
      assert(abs(slope.get_slope_from_p1_to_p2(position,position+tmp) -
                 tmp.inclination_angle()) < EPS);
    force += tmp;
  }
  acceleration = force / mass;
  assert(abs(slope.get_slope_from_p1_to_p2(position,position+acceleration) -
             acceleration.inclination_angle()) < EPS);
}

void Skier::update_velocity(double dtime, bool update_inclination)
{
  //if the cell is changed adjust old velocity inclination so that it still
  //lies on the plane of the slope
  if (update_inclination)
    velocity.rotate_axis_xy( velocity.inclination_angle() -
                             get_current_inclination_angle());

  velocity = velocity + acceleration * dtime;

  if (velocity.norm() < 1)
  {
    //assume that the minimum velocity is 1 m/s -> 3.6 Km/h, when the skier has
    //this velocity he is walking problably due to a counter slope
    velocity.normalize();
  }
}

void Skier::update_direction()
{
  if (velocity.norm() > 0)
    direction = velocity / velocity.norm();
}

bool Skier::fall_line_crossed() const
{
  double dir;
  double aspect = get_current_aspect();
  dir = direction.angle_on_xyplane();

  if (turning_left)
  {
    if (aspect > 0 && aspect < M_PI)
      return (dir > aspect && dir < aspect + M_PI);
    else
      return !(dir < aspect && dir > aspect - M_PI);
  }
  else
  {
    if (aspect > 0 && aspect < M_PI)
      return !(dir > aspect && dir < aspect + M_PI);
    else
      return (dir < aspect && dir > aspect - M_PI);
  }
}

double Skier::get_current_elevation() const
{
  return slope.get_elevation(position);
}

//returns the slope of the cell
double Skier::get_current_slope() const
{
  return slope.get_slope(position);
}

double Skier::get_current_aspect() const
{
  return slope.get_aspect(position);
}

//returns distance from left and intersection point
double Skier::current_distance_from_left(Point *p) const
{
  return slope.distance_from_left(position,p);
}

//returns distance from right and intersection point
double Skier::current_distance_from_right(Point *p) const
{
  return slope.distance_from_right(position,p);
}

bool Skier::is_inside_stop_area() const
{
  return slope.is_inside_stop_area(position);
}
