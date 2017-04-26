//
// Created by japata on 4/17/17.
//

#ifndef UNSCENTEDKALMANFILTER_BASEUKF_H
#define UNSCENTEDKALMANFILTER_BASEUKF_H

#include <vector>
#include <utility>
#include "Eigen/Dense"
#include "sensor_data_packet.h"

using Gaussian = std::pair<Eigen::VectorXd, Eigen::MatrixXd>;

/**
 * This class is a base class for an Unscented Kalman Filter. Meant to be inherited.
 *
 * Functions to implement:
 *  - InitializeState
 *  - PredictSigmaPoints
 *  - SigmaPointsToMeasurementSpace
 *  - ProcessSpaceMeanAndCovariance
 *  - MeasurementSpaceMeanAndCovariance
 *  - StateSpaceToCartesian
 *
 * @param n_states: The number of states that the UKF will track.
 * @param noise_stdevs: Vector of the noise values that correspond to the tracked states.
 * @param lambda: Scaling parameter for the sigma points.
 * */
class BaseUKF {
public:
    /**
     * Constructor and Destructor
     * */
    BaseUKF(int n_states, std::vector<float> noise_stdevs, double lambda);
    virtual ~BaseUKF() {}

    /**
     * These functions provide access to the current state and covariance from an initialized object.
     * */
    const Eigen::VectorXd &GetCurrentState() const { return x_; }
    const Eigen::MatrixXd &GetCurrentCovariance() const { return P_; }

    /**
     * Takes in a SensorDataPacket and performs all necessary actions to predict the state and update
     * based upon the measurement.
     *
     * @param data: Fully initialized SensorDataPacket.
     * */
    void ProcessMeasurement(SensorDataPacket &data);

protected:
    /**
     * Given a packet of sensor data, initialize the state of the UKF. Make sure to set is_initialized_ to true,
     * initialize prev_timestamp_, and return the state vector.
     *
     * @param data: The initial SensorDataPacket.
     * */
    virtual Eigen::VectorXd InitializeState(
            const SensorDataPacket &data) = 0;

    /**
     * This function needs to take the provided augmented sigma points and predict them forward dt seconds.
     *
     * @param sigma_pts: The provided sigma points.
     * @param delta_t: Time in seconds to predict forward.
     * */
    virtual Eigen::MatrixXd PredictSigmaPoints(
            const Eigen::MatrixXd &sigma_pts,
            const double delta_t) = 0;

    /**
     * This function needs to transform the provided sigma points into the space of the measurement. These
     * sigma points are the output of `PredictSigmaPoints`.
     *
     * @param sigma_pts: Predicted sigma points.
     * @param weights: The weights.
     * @param sensor_type: The type of sensor for the current observation.
     * */
    virtual Eigen::MatrixXd SigmaPointsToMeasurementSpace(
            const Eigen::MatrixXd &sigma_pts,
            const Eigen::VectorXd &weights,
            const SensorDataPacket::SensorType sensor_type) = 0;

    /**
     * This needs to calculate the mean and covariance matrix from the sigma points generated
     * by `PredictSigmaPoints`.
     *
     * @param sigma_pts: The sigma points generated by `PredictSigmaPoints`.
     * */
    virtual Gaussian ProcessSpaceMeanAndCovariance(
            const Eigen::MatrixXd &sigma_pts) = 0;

    /**
     * This needs to calculate the mean and covariance matrix from the sigma points generated
     * by `SigmaPointsToMeasurementSpace`.
     *
     * @param sigma_pts: The sigma points generated by `SigmaPointsToMeasurementSpace`.
     * @param mean: Store the calculated mean here.
     * @param store the calculated covariance here.
     * */
    virtual Gaussian MeasurementSpaceMeanAndCovariance(
            const Eigen::MatrixXd &sigma_pts,
            const SensorDataPacket::SensorType &sensor_type) = 0;

    /**
     * This function takes the current state, transforms it to cartesian coordinates, and returns it.
     *
     * @param x: The vector to transform.
     * */
    virtual Eigen::VectorXd StateSpaceToCartesian(
            const Eigen::VectorXd &x) = 0;

    /**
     * Normalizes a vector in state space.
     *
     * @param x: Un-normalized state vector.
     * */
    virtual Eigen::VectorXd NormalizeStateVector(
            const Eigen::VectorXd &x) = 0;

    /**
     * Normalizes a vector in measurement space.
     *
     * @param x: Un-normalized state vector.
     * */
    virtual Eigen::VectorXd NormalizeMeasurementVector(
            const Eigen::VectorXd &z,
            SensorDataPacket::SensorType sensor_type) = 0;

    const int n_states_;
    const int n_noise_coeffs_;
    const int n_aug_states_;
    bool is_initialized_;
    long long prev_timestamp_;

    // weights for prediction step
    Eigen::VectorXd weights_;

private:
    /**
     * Predict the current state and covariance forward in time dt seconds.
     *
     * @param data: A fully initialized SensorDataPacket.
     * @param dt: The time in seconds to forecast the state forward.
     * */
    void PredictState(const SensorDataPacket &data, const double dt);

    Eigen::VectorXd UpdateState(const Eigen::VectorXd &measurement, const SensorDataPacket::SensorType &sensor_type);

    /**
     * Use the provided state and covariance matrix to generate sigma points.
     *
     * @param x: State vector.
     * @param P: Covariance matrix.
     * */
    void GenerateSigmaPoints(const Eigen::VectorXd &x, const Eigen::MatrixXd &P);

    /**
     * Using the state and measurement space sigma points, mean, and covariance matrix, calculate the
     * Kalman gain matrix and store it in K_.
     *
     * @param sensor_type: Sensor type of the current measurement.
     * */
    void CalculateKalmanGain(const SensorDataPacket::SensorType sensor_type);

    int lambda_;

    // state vector
    Eigen::VectorXd x_;

    // state covariance
    Eigen::MatrixXd P_;

    // process noise covariance
    Eigen::MatrixXd P_aug_;

    // sigma points
    Eigen::MatrixXd X_sigma_points_;

    // measurement space sigma points
    Eigen::MatrixXd Z_sigma_points_;

    // measurement space mean
    Eigen::VectorXd z_;

    // measurement space covariance
    Eigen::MatrixXd S_;

    // kalman gain
    Eigen::MatrixXd K_;
};


#endif //UNSCENTEDKALMANFILTER_BASEUKF_H
