// Copyright (c) 2021 Michael Fabian Dirks <info@xaymar.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include "nvidia/cv/nvidia-cv.hpp"

#include "warning-disable.hpp"
#include <cinttypes>
#include "warning-enable.hpp"

#define P_NVAR_DEFINE_FUNCTION(name, ...)                           \
	private:                                                        \
	typedef ::streamfx::nvidia::cv::result (*t##name)(__VA_ARGS__); \
                                                                    \
	public:                                                         \
	t##name name = nullptr;

#define P_NVAR_INPUT "NvAR_Parameter_Input_"
#define P_NVAR_OUTPUT "NvAR_Parameter_Output_"
#define P_NVAR_CONFIG "NvAR_Parameter_Config_"

/*
 * Config Parameters:
 *	P_NVAR_CONFIG "BatchSize"
 *	P_NVAR_CONFIG "UseCudaGraph"
 *	P_NVAR_CONFIG "CUDAStream"
 *	P_NVAR_CONFIG "ExpressionCount"
 *	P_NVAR_CONFIG "FeatureDescription"
 *	P_NVAR_CONFIG "FocalLength"
 *	P_NVAR_CONFIG "GPU"
 *	P_NVAR_CONFIG "Landmarks_Size"
 *	P_NVAR_CONFIG "LandmarksConfidence_Size"
 *	P_NVAR_CONFIG "Mode"
 *	P_NVAR_CONFIG "TRTModelDir"
 *	P_NVAR_CONFIG "ModelDir"
 *	P_NVAR_CONFIG "ModelName"
 *	P_NVAR_CONFIG "NumKeyPoints"
 *	P_NVAR_CONFIG "ReferencePose"
 *	P_NVAR_CONFIG "ShapeEigenValueCount"
 *	P_NVAR_CONFIG "Temporal"
 *	P_NVAR_CONFIG "TriangleCount"
 *	P_NVAR_CONFIG "VertexCount"
 *
 * Input Parameters:
 *	P_NVAR_INPUT "Image"
 *	P_NVAR_INPUT "Width"
 *	P_NVAR_INPUT "Height"
 *	P_NVAR_INPUT "BoundingBoxes"
 *	P_NVAR_INPUT "BoundingBoxesConfidence"
 *	P_NVAR_INPUT "Landmarks"
 *
 * Output Parameters
 *	P_NVAR_OUTPUT "BoundingBoxes"
 *	P_NVAR_OUTPUT "BoundingBoxesConfidence"
 *	P_NVAR_OUTPUT "ExpressionCoefficients"
 *	P_NVAR_OUTPUT "FaceMesh"
 *	P_NVAR_OUTPUT "JointAngles"
 *	P_NVAR_OUTPUT "KeyPoints"
 *	P_NVAR_OUTPUT "KeyPoints3D"
 *	P_NVAR_OUTPUT "KeyPointsConfidence"
 *	P_NVAR_OUTPUT "Landmarks"
 *	P_NVAR_OUTPUT "LandmarksConfidence"
 *	P_NVAR_OUTPUT "Pose"
 *	P_NVAR_OUTPUT "RenderingParams"
 *	P_NVAR_OUTPUT "ShapeEigenValues"
 */

namespace streamfx::nvidia::ar {
	typedef const char* feature_t;
	typedef const char* parameter_t;
	typedef void*       object_t;
	typedef void*       handle_t;

	static constexpr feature_t FEATURE_BODY_DETECTION       = "BodyDetection";
	static constexpr feature_t FEATURE_BODY_POSE_ESTIMATION = "BodyPoseEstimation";
	static constexpr feature_t FEATURE_FACE_DETECTION       = "FaceDetection";
	static constexpr feature_t FEATURE_FACE_BOX_DETECTION   = "FaceBoxDetection";
	static constexpr feature_t FEATURE_FACE_RECONSTRUCTION  = "Face3DReconstruction";
	static constexpr feature_t FEATURE_LANDMARK_DETECTION   = "LandMarkDetection";

	template<typename T>
	struct vec2 {
		T x;
		T y;
	};

	template<typename T>
	struct vec3 : public vec2<T> {
		T z;
	};

	template<typename T>
	struct vec4 : public vec3<T> {
		T w;
	};

	typedef vec2<float> point_t;
	typedef vec4<float> frustum_t;
	typedef vec4<float> quaternion_t;
	typedef vec4<float> rect_t;

	struct bounds_t {
		rect_t* rects;
		uint8_t current;
		uint8_t maximum;
	};

	struct face_mesh_t {
		vec3<float>*   vertices;
		size_t         num_vertices;
		vec3<uint16_t> indices;
		size_t         num_indices;
	};

	struct rendering_params_t {
		frustum_t    frustum;
		quaternion_t rotation;
		vec3<float>  translation;
	};

	class ar {
		std::shared_ptr<::streamfx::util::library> _library;
		std::filesystem::path                      _model_path;
#ifdef WIN32
		void* _extra;
#endif

		public:
		~ar();
		ar();

		std::filesystem::path const& get_model_path();

		public:
		P_NVAR_DEFINE_FUNCTION(NvAR_GetVersion, uint32_t* version);

		P_NVAR_DEFINE_FUNCTION(NvAR_Create, feature_t feature_id, handle_t* ptr);
		P_NVAR_DEFINE_FUNCTION(NvAR_Destroy, handle_t ptr);
		P_NVAR_DEFINE_FUNCTION(NvAR_Run, handle_t ptr);
		P_NVAR_DEFINE_FUNCTION(NvAR_Load, handle_t ptr);

		P_NVAR_DEFINE_FUNCTION(NvAR_GetS32, handle_t ptr, parameter_t parameter, int32_t* value);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetS32, handle_t ptr, parameter_t parameter, int32_t value);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetU32, handle_t ptr, parameter_t parameter, uint32_t* value);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetU32, handle_t ptr, parameter_t parameter, uint32_t value);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetU64, handle_t ptr, parameter_t parameter, uint64_t* value);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetU64, handle_t ptr, parameter_t parameter, uint64_t value);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetF32, handle_t ptr, parameter_t parameter, float* value);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetF32, handle_t ptr, parameter_t parameter, float value);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetF64, handle_t ptr, parameter_t parameter, double* value);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetF64, handle_t ptr, parameter_t parameter, double value);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetString, handle_t ptr, parameter_t parameter, const char** value);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetString, handle_t ptr, parameter_t parameter, const char* value);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetCudaStream, handle_t ptr, parameter_t parameter,
							   ::streamfx::nvidia::cuda::stream_t* value);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetCudaStream, handle_t ptr, parameter_t parameter,
							   ::streamfx::nvidia::cuda::stream_t value);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetObject, handle_t ptr, parameter_t parameter, object_t* value, uint32_t size);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetObject, handle_t ptr, parameter_t parameter, object_t value, uint32_t size);
		P_NVAR_DEFINE_FUNCTION(NvAR_GetF32Array, handle_t ptr, parameter_t parameter, const float** values,
							   int32_t* size);
		P_NVAR_DEFINE_FUNCTION(NvAR_SetF32Array, handle_t ptr, parameter_t parameter, const float* values,
							   int32_t size);

		public:
		static std::shared_ptr<::streamfx::nvidia::ar::ar> get();
	};
} // namespace streamfx::nvidia::ar
