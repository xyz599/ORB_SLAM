/*
 * =====================================================================================
 *
 *       Filename:  ProbabilityMapping.h
 *
 *    Description:  Semi-dense Probability Mapping Module for ORB-SLAM
 *    inspired by Raul-Mur Artal's paper
 *
 *        Version:  0.01
 *        Created:  01/21/2016 03:48:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Authors: Josh Tang, Rebecca Frederick 
 *   Organization:  Unkei
 *
 * =====================================================================================
 */

#ifndef PROBABILITYMAPPING_H
#define PROBABILITYMAPPING_H

#define N 7
#define sigmaI 20
#define lambdaG 8
#define lambdaL 80
#define lambdaTheta 45
#define lambdaN 3
#define histo_length 30
#define th_high = 100
#define th_low = 50
#define nnratio 0.6

class ProbabilityMapping {

private:

	struct depthHo {
		float depth;
		float sigma;
	};

        void ComputeInvDepthHypothesis(const KeyFrame* kf, int pixel, float ustar, float ustar_var, float a, float b, float c, depthHo& dh);
        void GetImageGradient(const cv::Mat& image, cv::Mat* gradx, cv::Mat* grady, cv::Mat* grad);
        void GetGradientOrientation(int x, int y, const cv::Mat& gradx, const cv::Mat& grady, float* th);
        void GetInPlaneRotation(const ORB_SLAM::KeyFrame* k1, const ORB_SLAM::KeyFrame* k2, float* th);
        void GetIntensityGradient(cv::Mat im, float* g);
        void PixelNeighborSuppport(const depthHo** H, int x, int y, std::vector<depthHo>* support);
        void PixelNeighborNeighborSupport(const depthHo** H, int px, int py, std::vector<std::vector<depthHo>* support);
        void GetIntensityGradient_D(const cv::Mat& im, float* q);				
        void GetPixelDepth(const cv::Mat& Im, const cv::Mat& R, const cv::Mat& T, const ORB_SLAM::KeyFrame* kF, int u, float *p);
	bool ChiTest(const depthHo& ha, const depthHo& hb, float* chi_val);
	void GetFusion(const std::vector<depthHo>& best_compatible_ho, depthHo* hypothesis, float* min_sigma);

public:

	/* * \brief void first_loop(KeyFrame kf, depthHo**, std::vector<depthHo>&): return results of epipolar search (depth hypotheses) */
	void FirstLoop(const KeyFrame *kf, depthHo**, std::vector<depthHo>&);
        /* * \brief void stereo_search_constraints(): return min, max inverse depth */
        void StereoSearchConstraints(const KeyFrame *kf, float* min_depth, float* max_depth);
	/* * \brief void epipolar_search(): return distribution of inverse depths/sigmas for each pixel */
        void EpipolarSearch(const KeyFrame *kf1, const Keyframe *kf2, int x, int y, cv::Mat gradx, cv::Mat grady, cv::Mat grad, float min_depth, float max_depth, depthHo* dh);
	/* * \brief void inverse_depth_hypothesis_fusion(const vector<depthHo> H, depthHo* dist): 
	 * *         get the parameters of depth hypothesis distrubution from list of depth hypotheses */
        void InverseDepthHypothesisFusion(const std::vector<depthHo>& h, depthHo* dist);
	/* * \brief void intraKeyframeDepthChecking(depthHo** h, int imrows, int imcols): intra-keyframe depth-checking, smoothing, and growing. */
        void IntraKeyframeDepthChecking(depthHo** h, int imrows, int imcols);
	/* * \brief void interKeyframeDepthChecking(const ORB_SLAM::KeyFrame* currentKF, depthHo** h, int imrows, int imcols): 
         * *         inter-keyframe depth-checking, smoothing, and growing. */
        void InterKeyframeDepthChecking(const ORB_SLAM::KeyFrame* currentKF, depthHo** h, int imrows, int imcols);

};

#endif
