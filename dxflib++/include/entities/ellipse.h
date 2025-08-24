#pragma once
#include "dxflib++/include/entities/point.h"
#include "dxflib++/include/entities/entity.h"

namespace dxflib::entities
{
	namespace group_codes
	{
		enum class ellipse
		{
			center_point_x = 10,
			center_point_y = 20,
			center_point_z = 30,
			end_point_major_axis_x = 11,
			end_point_major_axis_y = 21,
			end_point_major_axis_z = 31,
			ratio_minor_to_major = 40,
			start_param = 41,
			end_param = 42
		};
	}

	struct ellipse_buffer : virtual entity_buffer_base
	{
		double center_point_x{};
		double center_point_y{};
		double center_point_z{};
		double end_point_major_axis_x{};
		double end_point_major_axis_y{};
		double end_point_major_axis_z{};
		double ratio_minor_to_major{};
		double start_param{};
		double end_param{};

		int parse(const std::string& cl, const std::string& nl) override;
		void free() override;
	};

	// ReSharper disable CppPolymorphicClassWithNonVirtualPublicDestructor
	/**
	 * \brief ELLIPSE entity 
	 */
	class ellipse : entity
	{
	public:
		// Constructor
		explicit ellipse(ellipse_buffer& ab); // buffer Constructor

		// Public Interface
		// Get
		const vertex& get_center_point() const { return center_point_; } // Returns the Center Point
		const vertex& get_end_point_major_axis() const { return end_point_major_axis_; } // Returns the End point of major axis
		double get_ratio_minor_to_major() const { return ratio_minor_to_major_; } // Returns the Ratio of minor axis to major axis
		double get_start_param() const { return start_param_; } // Returns the Starting param
		double get_end_param() const { return end_param_; } // Returns the Final param

	private:
		// Properties
		vertex center_point_; // Center point of the ellipse
		vertex end_point_major_axis_; // Endpoint of major axis, relative to the center (in WCS)
		double ratio_minor_to_major_; // Ratio of minor axis to major axis
		double start_param_; //(this value is 0.0 for a full ellipse)
		double end_param_; // (this value is 2pi for a full ellipse)
	};
}
