/*
 * circle.cpp
 * By: Cheungyung NGAI
 * cheungyungngai@flexus3d.com
 * 2022-07-07
 * Purpose: To define the decarlations made in the circle.h file for the circle_buffer and the
 * circle enity class
 */
#include "dxflib++/include/entities/circle.h"
#include "dxflib++/include/mathlib.h"
#include <valarray>

int dxflib::entities::circle_buffer::parse(const std::string& cl, const std::string& nl)
{
  int code{entity_buffer_base::parse(cl, nl)}; // group code of the current line
  if (code == -1) return 1;
  // Parse switch;
  switch (static_cast<group_codes::circle>(code))
  {
  case group_codes::circle::thickness:
    thickness = std::stod(nl);
    return 1;

  case group_codes::circle::center_point_x:
    center_point_x = std::stod(nl);
    return 1;

  case group_codes::circle::center_point_y:
    center_point_y = std::stod(nl);
    return 1;

  case group_codes::circle::center_point_z:
    center_point_z = std::stod(nl);
    return 1;

  case group_codes::circle::radius:
    radius = std::stod(nl);
    return 1;


  case group_codes::circle::line_type:
    line_type = nl;
    return 1;

  case group_codes::circle::line_type_scale:
    line_type_scale = std::stod(nl);
    return 1;

  default:
    return 0;
  }
}

void dxflib::entities::circle_buffer::free()
{
  entity_buffer_base::free();
  thickness = 0;
  center_point_x = 0;
  center_point_y = 0;
  center_point_z = 0;
  radius = 0;
  line_type = "";
  line_type_scale = 1.0;
}

dxflib::entities::circle::circle(circle_buffer& cb): entity(cb),
center_point_(cb.center_point_x, cb.center_point_y, cb.center_point_z),
radius_(cb.radius), thickness_(cb.thickness),

line_type(cb.line_type),
line_type_scale(cb.line_type_scale)
{
}
