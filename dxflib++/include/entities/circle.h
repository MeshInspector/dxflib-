/*
 * circle.h
 * By: Cheungyung NGAI
 * cheungyungngai@flexus3d.com
 *
 * 2022-07-07
 * Purpose: To define the decarlations made in the circle.h file for the circle_buffer and the
 * circle enity class
 */

#pragma once

#include "dxflib++/include/entities/point.h"
#include "dxflib++/include/entities/entity.h"

namespace dxflib::entities
{
  namespace group_codes
  {
    enum class circle
    {
      thickness        = 39,
      color            = 62,
      line_type        = 6,  // Line Type
      line_type_scale  = 48, // Linetype scale
      center_point_x = 10,
      center_point_y = 20,
      center_point_z = 30,

      radius = 40

    };
  }

  struct circle_buffer: virtual entity_buffer_base
  {
    double center_point_x{};
    double center_point_y{};
    double center_point_z{};
    double thickness{};
    double radius{};
    double line_type_scale{};
    std::string line_type;

    int parse(const std::string& cl, const std::string& nl) override;
    void free() override;
  };

  // ReSharper disable CppPolymorphicClassWithNonVirtualPublicDestructor
  /**
   * \brief CIRCLE entity
   */
  class circle: entity
  {
  public:
    // Constructor
    explicit circle(circle_buffer& ab); // buffer Constructor

    // Public Interface
    // Get
    const vertex& get_center_point() const { return center_point_; } // Returns the Center Point
    double get_radius() const { return radius_; }                    // Returns the Radius
    double get_thickness() const { return thickness_; }              // Returns the Thickness

  private:
    // Properties
    vertex center_point_;    // Center point of the arc
    double radius_;          // radius of the arc
    double thickness_;       // thickness of the line that is the arc
    double line_type_scale;
    std::string line_type;

  };
}
