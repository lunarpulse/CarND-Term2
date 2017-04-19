//
// Created by japata on 4/17/17.
//

#ifndef UNSCENTEDKALMANFILTER_BASEEKF_H
#define UNSCENTEDKALMANFILTER_BASEEKF_H

#include <vector>
#include "Eigen/Dense"
#include "sensor_data_packet.h"


class BaseEKF {
public:
    BaseEKF(int n_states, std::vector<double> noise_stdevs, double lambda);

    Eigen::VectorXd GetCurrentState() const { return x_; }
    Eigen::MatrixXd GetCurrentCovariance() const { return P_; }
    void ProcessMeasurement(SensorDataPacket &data);

protected:
    virtual void PredictSigmaPoints(Eigen::MatrixXd &sigma_pts, const double delta_t);
    virtual void SigmaPointsToMeasurementSpace(Eigen::MatrixXd &sigma_pts,
                                               const Eigen::VectorXd &weights,
                                               const SensorDataPacket::SensorType sensor_type);

    int n_states_;
    int n_aug_states_;

    // state vector
    Eigen::VectorXd x_;

    // state covariance
    Eigen::MatrixXd P_;

private:
    void GenerateSigmaPoints(const Eigen::VectorXd &x, const Eigen::MatrixXd &P);
    void GetMeanAndCovariance();

    int lambda_;
    bool is_initialized_;
    long long prev_timestamp_;

    // process noise covariance
    Eigen::MatrixXd Q_;

    // sigma points
    Eigen::MatrixXd sigma_points_;

    // weights for prediction step
    Eigen::VectorXd weights_;
};


#endif //UNSCENTEDKALMANFILTER_BASEEKF_H