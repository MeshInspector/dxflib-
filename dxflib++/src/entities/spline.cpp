#include "dxflib++/include/entities/spline.h"
#include "dxflib++/include/mathlib.h"
#include <assert.h>
#include <iostream>

// splinebuf parse
/**
 * \brief Parse Function for the line buffer
 * \param cl Current line
 * \param nl Next line
 * \return status -> 1: sucess, 0: no success
 */
int dxflib::entities::spline_buf::parse(const std::string& cl, const std::string& nl)
{
	using namespace group_codes;
	int code{entity_buffer_base::parse(cl, nl)};
	if (code == -1)
		return 1;
	// Switch on the code and set the aproprate value in the line buffer
	switch (static_cast<spline_codes>(code))
	{
	case spline_codes::error:
		return 0;
	case spline_codes::normal_vector_x:
		normal_vector_x = std::stod(nl);
		return 1;
	case spline_codes::normal_vector_y:
		normal_vector_y = std::stod(nl);
		return 1;
	case spline_codes::normal_vector_z:
		normal_vector_z = std::stod(nl);
		return 1;
	case spline_codes::spline_flag:
		spline_flag = std::stoi(nl);
		return 1;
	case spline_codes::degree_of_spline_curve:
		degree_of_spline_curve = std::stoi(nl);
		return 1;
	case spline_codes::number_of_knots:
		number_of_knots = std::stoi(nl);
		return 1;
	case spline_codes::number_of_control_points:
		number_of_control_points = std::stoi(nl);
		return 1;
	case spline_codes::number_of_fit_points:
		number_of_fit_points = std::stoi(nl);
		return 1;
	case spline_codes::knot_tolerance:
		knot_tolerance = std::stod(nl);
		return 1;
	case spline_codes::control_point_tolerance:
		control_point_tolerance = std::stod(nl);
		return 1;
	case spline_codes::fit_tolerance:
		fit_tolerance = std::stod(nl);
		return 1;

	case spline_codes::start_tangent_x:
		has_start_tangent = true;
		start_tangent_x = std::stod(nl);
		return 1;
	case spline_codes::start_tangent_y:
		start_tangent_y = std::stod(nl);
		return 1;
	case spline_codes::start_tangent_z:
		start_tangent_z = std::stod(nl);
		return 1;
	case spline_codes::end_tangent_x:
		has_end_tangent = true;
		end_tangent_x = std::stod(nl);
		return 1;
	case spline_codes::end_tangent_y:
		end_tangent_y = std::stod(nl);
		return 1;
	case spline_codes::end_tangent_z:
		end_tangent_z = std::stod(nl);
		return 1;
	case spline_codes::knot_value:
		knotValues.emplace_back(std::stod(nl));
		return 1;
	case spline_codes::weight:
		weights.emplace_back(std::stod(nl));
		return 1;
	case spline_codes::control_point_x:
		control_points_x_values.emplace_back(std::stod(nl));
		return 1;
	case spline_codes::control_point_y:
		control_points_y_values.emplace_back(std::stod(nl));
		return 1;
	case spline_codes::control_point_z:
		control_points_z_values.emplace_back(std::stod(nl));
		return 1;
	case spline_codes::fit_point_x:
		fit_points_x_values.emplace_back(std::stod(nl));
		return 1;
	case spline_codes::fit_point_y:
		fit_points_y_values.emplace_back(std::stod(nl));
		return 1;
	case spline_codes::fit_point_z:
		fit_points_z_values.emplace_back(std::stod(nl));
		return 1;
	default:
		return 0;
	}
}

void dxflib::entities::spline_buf::free()
{
	entity_buffer_base::free();

	normal_vector_x = 0.;
	normal_vector_y = 0.;
	normal_vector_z = 0.;

	spline_flag = 0;
	degree_of_spline_curve = 0;
	number_of_knots = 0;
	number_of_control_points = 0;
	number_of_fit_points = 0;

	knot_tolerance = knot_tolerance_default;
	control_point_tolerance = control_point_tolerance_default;
	fit_tolerance = fit_tolerance_default;

	has_start_tangent = false;
	start_tangent_x = 0.;
	start_tangent_y = 0.;
	start_tangent_z = 0.;

	has_end_tangent = false;
	end_tangent_x = 0.;
	end_tangent_y = 0.;
	end_tangent_z = 0.;

	knotValues.clear();
	weights.clear();

	control_points_x_values.clear();
	control_points_y_values.clear();
	control_points_z_values.clear();

	fit_points_x_values.clear();
	fit_points_y_values.clear();
	fit_points_z_values.clear();
}

static std::vector<dxflib::entities::vertex> merge_coordinates_helper(
	const std::vector<double>& x_points,
	const std::vector<double>& y_points,
	const std::vector<double>& z_points)
{
	std::vector<dxflib::entities::vertex> merged;
	const size_t N_x = x_points.size();
	const size_t N_y = y_points.size();
	const size_t N_z = z_points.size();

	//What if they are different in size?
	assert(N_x == N_y);
	assert(N_x == N_z);

	for (size_t i = 0; i < N_x; ++i)
	{
		const double& x = x_points[i];
		const double y = i < N_y ? y_points[i] : 0.;
		const double z = i < N_z ? z_points[i] : 0.;

		merged.emplace_back(x, y, z);
	}

	return merged;
}

/**
 * \brief Line buffer constructor for the line entity
 * \param lb Line Buffer
 */
dxflib::entities::spline::spline(spline_buf& b) :
	entity(b),
	normal_vector_(b.normal_vector_x, b.normal_vector_y, b.normal_vector_z),
	spline_flag_(b.spline_flag),
	degree_of_spline_curve_(b.degree_of_spline_curve),
	number_of_knots_(b.number_of_knots),
	number_of_control_points_(b.number_of_control_points),
	number_of_fit_points_(b.number_of_fit_points),
	knot_tolerance_(b.knot_tolerance),
	control_point_tolerance_(b.control_point_tolerance),
	fit_tolerance_(b.fit_tolerance),
	has_start_tangent_(b.has_start_tangent),
	start_tangent_(b.start_tangent_x, b.start_tangent_y, b.start_tangent_z),
	has_end_tangent_(b.has_end_tangent),
	end_tangent_(b.end_tangent_x, b.end_tangent_y, b.end_tangent_z),
	knot_values_(b.knotValues),
	weights_(b.weights)
{
	control_points_ = merge_coordinates_helper(b.control_points_x_values, b.control_points_y_values, b.control_points_z_values);
	assert((size_t)number_of_control_points_ == control_points_.size());

	fit_points_ = merge_coordinates_helper(b.fit_points_x_values, b.fit_points_y_values, b.fit_points_z_values);
	assert((size_t)number_of_fit_points_ == fit_points_.size());

	//not clear about weights vector if not empty.
	assert(number_of_knots_ == knot_values_.size());
}

const dxflib::entities::vertex& dxflib::entities::spline::get_normal_vector() const
{
	return normal_vector_;
}

const std::vector<dxflib::entities::vertex>& dxflib::entities::spline::get_control_points() const
{
	return control_points_;
}

const std::vector<dxflib::entities::vertex>& dxflib::entities::spline::get_fit_points() const
{
	return fit_points_;
}

const std::vector<double>& dxflib::entities::spline::get_knot_values() const
{
	return knot_values_;
}

const std::vector<double>& dxflib::entities::spline::get_weights() const
{
	return weights_;
}

double dxflib::entities::spline::get_knot_tolerance() const
{
	return knot_tolerance_;
}

double dxflib::entities::spline::get_control_point_tolerance() const
{
	return control_point_tolerance_;
}

double dxflib::entities::spline::get_fit_tolerance() const
{
	return fit_tolerance_;
}

unsigned int dxflib::entities::spline::get_spline_flag() const
{
	return spline_flag_;
}

unsigned int dxflib::entities::spline::get_degree_of_spline_curve() const
{
	return degree_of_spline_curve_;
}

bool dxflib::entities::spline::has_start_tangent() const
{
	return has_start_tangent_;
}

const dxflib::entities::vertex& dxflib::entities::spline::get_start_tangent() const
{
	return start_tangent_;
}

bool dxflib::entities::spline::has_end_tangent() const
{
	return has_end_tangent_;
}

const dxflib::entities::vertex& dxflib::entities::spline::get_end_tangent() const
{
	return end_tangent_;
}
