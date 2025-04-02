#pragma once
#include "dxflib++/include/entities/entity.h"
#include "dxflib++/include/entities/point.h"

namespace dxflib::entities
{
	/**
	* \brief Spline buffer struct
	*/
	struct spline_buf : entity_buffer_base
	{
		double normal_vector_x = 0.;
		double normal_vector_y = 0.;
		double normal_vector_z = 0.;

		unsigned int spline_flag = 0;
		unsigned int degree_of_spline_curve = 0;
		unsigned int number_of_knots = 0;
		unsigned int number_of_control_points = 0;
		unsigned int number_of_fit_points = 0;

		static constexpr double knot_tolerance_default = 0.0000001;
		static constexpr double control_point_tolerance_default = 0.0000001;
		static constexpr double fit_tolerance_default = 0.0000000001;//another! See docs.

		double knot_tolerance = knot_tolerance_default;
		double control_point_tolerance = control_point_tolerance_default;
		double fit_tolerance = fit_tolerance_default;

		bool has_start_tangent = false;
		double start_tangent_x = 0.;
		double start_tangent_y = 0.;
		double start_tangent_z = 0.;

		bool has_end_tangent = false;
		double end_tangent_x = 0.;
		double end_tangent_y = 0.;
		double end_tangent_z = 0.;

		std::vector<double> knotValues;
		std::vector<double> weights;

		std::vector<double> control_points_x_values;
		std::vector<double> control_points_y_values;
		std::vector<double> control_points_z_values;

		std::vector<double> fit_points_x_values;
		std::vector<double> fit_points_y_values;
		std::vector<double> fit_points_z_values;

		/**
		 * \brief Parse function for line buffer
		 * \param cl Current Line
		 * \param nl Next Line
		 * \return Status 1:pass, 2: fail.
		 */
		int parse(const std::string& cl, const std::string& nl) override;
		/**
		 * \brief Frees memory and resets to defaults
		 */
		void free() override;
	};

	namespace group_codes
	{
		enum class spline_codes
		{
			subclassmarker = 100,
			error = -1,

			normal_vector_x = 210,
			normal_vector_y = 220,
			normal_vector_z = 230,

			spline_flag = 70,
			degree_of_spline_curve = 71,
			number_of_knots = 72,
			number_of_control_points = 73,
			number_of_fit_points = 74,

			knot_tolerance = 42,
			control_point_tolerance = 43,
			fit_tolerance = 44,

			start_tangent_x = 12,
			start_tangent_y = 22,
			start_tangent_z = 32,

			end_tangent_x = 13,
			end_tangent_y = 23,
			end_tangent_z = 33,

			knot_value = 40,
			weight = 41,

			control_point_x = 10,
			control_point_y = 20,
			control_point_z = 30,

			fit_point_x = 11,
			fit_point_y = 21,
			fit_point_z = 31
		};
	}

	/**
	* \brief Spline Entity
	*/
	// ReSharper disable CppPolymorphicClassWithNonVirtualPublicDestructor
	class spline : public entity
	{
	public:
		// Constructor
		explicit spline(spline_buf& b);

		const vertex& get_normal_vector() const;
		const std::vector<vertex>& get_control_points() const;
		const std::vector<vertex>& get_fit_points() const;
		const std::vector<double>& get_knot_values() const;
		const std::vector<double>& get_weights() const;

		double get_knot_tolerance() const;
		double get_control_point_tolerance() const;
		double get_fit_tolerance() const;

		unsigned int get_spline_flag() const;
		unsigned int get_degree_of_spline_curve() const;

		bool has_start_tangent() const;
		const vertex& get_start_tangent() const;
		bool has_end_tangent() const;
		const vertex& get_end_tangent() const;
	private:
		// Properties
		vertex normal_vector_;

		unsigned int spline_flag_ = 0;
		unsigned int degree_of_spline_curve_ = 0;
		unsigned int number_of_knots_ = 0;
		unsigned int number_of_control_points_ = 0;
		unsigned int number_of_fit_points_ = 0;

		double knot_tolerance_ = 0.;
		double control_point_tolerance_ = 0.;
		double fit_tolerance_ = 0.;

		bool has_start_tangent_ = false;
		vertex start_tangent_;

		bool has_end_tangent_ = false;
		vertex end_tangent_;

		std::vector<double> knot_values_;
		std::vector<double> weights_;

		std::vector<vertex> control_points_;
		std::vector<vertex> fit_points_;
	};
}
