
// Copyright (c) 2012 Christopher Lux <christopherlux@gmail.com>
// Distributed under the Modified BSD License, see license.txt.

#include <limits>
#include <vector>
#include <scm/core/math/math.h>

#include <scm/data/volume/volume_filter_kernel_sobel3x3.h>

namespace scm {
namespace data {

template<typename output_voxel_component_type, typename input_data_type>
bool
volume_gradient_calculator<output_voxel_component_type,
                           input_data_type>::generate_gradients(unsigned int width,
                            unsigned int height,
                            unsigned int depth,
                            const input_data_type*const src_data,
                            output_voxel_component_type*const dst_buffer)
{
    if (   !src_data
        || !dst_buffer
        || width == 0
        || height == 0
        || depth == 0) {
        return (false);
    }

    _width  = width;
    _height = height;
    _depth  = depth;

    bool i_int = std::numeric_limits<input_data_type>::is_integer;
    input_data_type i_max = (std::numeric_limits<input_data_type>::max)();
    bool i_sgn = std::numeric_limits<input_data_type>::is_signed;

    bool o_int = std::numeric_limits<output_voxel_component_type>::is_integer;
    output_voxel_component_type o_max = (std::numeric_limits<output_voxel_component_type>::max)();
    bool o_sgn = std::numeric_limits<output_voxel_component_type>::is_signed;

    // currently no unsigned gradient output
    // gradient can be packed into signed output, but with loss of precission
    //if (!o_sgn) {
    //    return (false);
    //}

    //// output buffer, 4 components.
    //// gradient_x, gradient_y, gradient_z, value
    //output_voxel_component_type* dst_buffer = new output_voxel_component_type[width*height*depth*4];

    // internally gradients are calculated in float precission
    std::vector<float> cur_data;
    cur_data.resize(27); // make room for 27 elements

    // filter kernels
    volume_filter_kernel_sobel3x3_x sobel_x;
    volume_filter_kernel_sobel3x3_y sobel_y;
    volume_filter_kernel_sobel3x3_z sobel_z;

    scm::math::vec3f tmp_grad;
    //float tmp_dx;
    //float tmp_dy;
    //float tmp_dz;

    for (unsigned int z = 0; z < depth; z++) {
        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {

                // read current data for the kernels to process
                for (unsigned int kz = 0; kz < 3; kz++) {
                    for (unsigned int ky = 0; ky < 3; ky++) {
                        for (unsigned int kx = 0; kx < 3; kx++) {

                            cur_data[kx + ky*3 + kz*9] = src_data[get_offset_clamp_to_edge(x + kx - 1, y + ky - 1, z + kz - 1)];

                            if (i_int && !o_int) {
                                cur_data[kx + ky*3 + kz*9] /= (float)i_max;
                            }

                        }
                    }
                }

                tmp_grad.x = sobel_x.apply(cur_data);
                tmp_grad.y = sobel_y.apply(cur_data);
                tmp_grad.z = sobel_z.apply(cur_data);

                if (o_int) {
                    scm::math::vec<output_voxel_component_type, 3> packed_grad;
                    tmp_grad /= scm::math::sqrt(3.0f);//normalize(tmp_grad);

                    if (!o_sgn) {
                        packed_grad.x = static_cast<output_voxel_component_type>((tmp_grad.x + 255.0f) * 0.5f);
                        packed_grad.y = static_cast<output_voxel_component_type>((tmp_grad.y + 255.0f) * 0.5f);
                        packed_grad.z = static_cast<output_voxel_component_type>((tmp_grad.z + 255.0f) * 0.5f);
                    }
                    else {
                        packed_grad.x = static_cast<output_voxel_component_type>(tmp_grad.x * o_max);
                        packed_grad.y = static_cast<output_voxel_component_type>(tmp_grad.y * o_max);
                        packed_grad.z = static_cast<output_voxel_component_type>(tmp_grad.z * o_max);
                    }

                    dst_buffer[get_output_offset(x, y, z) + 0] = packed_grad.x;
                    dst_buffer[get_output_offset(x, y, z) + 1] = packed_grad.y;
                    dst_buffer[get_output_offset(x, y, z) + 2] = packed_grad.z;

                    dst_buffer[get_output_offset(x, y, z) + 3] = cur_data[14];
                }
                else {
                    dst_buffer[get_output_offset(x, y, z) + 0] = tmp_grad.x;
                    dst_buffer[get_output_offset(x, y, z) + 1] = tmp_grad.y;
                    dst_buffer[get_output_offset(x, y, z) + 2] = tmp_grad.z;

                    dst_buffer[get_output_offset(x, y, z) + 3] = cur_data[14];
                }

                // to do!
                //if (i_int && o_int) {
                //    if (i_max < o_max) {

                //    }
                //    if (i_max > o_max) {

                //    }
                //}
                // end to do
            }
        }
    }

    return (true);
}

template<typename output_voxel_component_type, typename input_data_type>
unsigned int
volume_gradient_calculator<output_voxel_component_type,
                           input_data_type>::get_offset_clamp_to_edge(int x,
                                                                      int y,
                                                                      int z)
{
    unsigned int offset;

    offset =   scm::math::clamp(x, 0, int(_width)  - 1)
             + scm::math::clamp(y, 0, int(_height) - 1) * _width
             + scm::math::clamp(z, 0, int(_depth)  - 1) * _width * _height;

    return (offset);
}

template<typename output_voxel_component_type, typename input_data_type>
unsigned int
volume_gradient_calculator<output_voxel_component_type,
                           input_data_type>::get_output_offset( unsigned int x,
                                                                unsigned int y,
                                                                unsigned int z)
{
    unsigned int offset;

    offset =   x * 4
             + y * _width * 4
             + z * _width * _height * 4;

    return (offset);
}

} // namespace data
} // namespace scm
