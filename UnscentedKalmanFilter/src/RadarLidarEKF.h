//
// Created by jacob on 4/18/2017.
//

#ifndef UNSCENTEDKALMANFILTER_RADARLIDAREKF_H
#define UNSCENTEDKALMANFILTER_RADARLIDAREKF_H


#include "BaseEKF.h"

class RadarLidarEKF : BaseEKF
{
public:
    RadarLidarEKF::RadarLidarEKF(
            std::vector<float> radar_noise,
            std::vector<float> lidar_noise,
            std::vector<float> process_noise,
            double lambda);

private:
    Eigen::MatrixXd PredictSigmaPoints(
            const Eigen::MatrixXd &sigma_pts,
            const double delta_t);

    Eigen::MatrixXd SigmaPointsToMeasurementSpace(
            const Eigen::MatrixXd &sigma_pts,
            const Eigen::VectorXd &weights,
            const SensorDataPacket::SensorType sensor_type);

    void ProcessSpaceMeanAndCovariance(
            const Eigen::MatrixXd &sigma_pts,
            Eigen::VectorXd &mean,
            Eigen::MatrixXd &cov);

    void MeasurementSpaceMeanAndCovariance(
            const Eigen::MatrixXd &sigma_pts,
            const SensorDataPacket::SensorType &sensor_type,
            Eigen::VectorXd &mean,
            Eigen::MatrixXd &cov);

    Eigen::MatrixXd R_radar_;
    Eigen::MatrixXd R_lidar_;
};


#endif //UNSCENTEDKALMANFILTER_RADARLIDAREKF_H
