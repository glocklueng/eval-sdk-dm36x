/*
 * Copyright 2010 RidgeRun
 */

#ifndef RR_AEW
#define RR_AEW
#include <linux/types.h>
#include <config.h>
/** @file rraew.h */

/**
 * Aauto white balance algorithm to use.
 */
enum rraew_white_balance_algo {
    AWB_NONE = 0,/**<No algorithm, disables auto white balance.*/
    AWB_GRAY_WORLD,  /**<Gray World algorithm is based on the assumption 
    that given an image with sufficient amount of color variation, the average 
    reflectance of the scene is achromatic (gray).*/
    AWB_WHITE_PATCH, /**<White Patch algorithm assumes that the maximum 
    response in an image is caused by a perfect reflector, so represents the 
    color of the illumination based on the absolute maximum.*/
    AWB_WHITE_PATCH_2 /**<White Patch algorithm assumes that the maximum 
    response in an image is caused by a perfect reflector, so represents the 
    color of the illumination based on the average of local maximums.*/
};
/**
 * Auto exposure algorithm to use.
 */
enum rraew_exposure_algo {
    AE_NONE, /**<No algorithm, disables auto exposure.*/
    AE_EC /**< Electronic centric (EC) algorithm adjusts the 
    brightness level to the mid-tone.*/
};
/**
 * Brightness metering system to be used by the auto exposure algorithm.
 */
enum rraew_metering_type {
    METER_PARTIAL_AREA = 0, /**<Average the light information coming from a
    frame portion in a user defined area.*/
    METER_RECT_WEIGHTED, /**<Average the light information coming from the 
    entire frame with emphasis placed on a user defined area.*/
    METER_AVERAGE, /**<Average the light information coming from the entire
    frame without any location based weighting.*/
    METER_SEGMENT /**<Divides the frame into six pieces and weights them to
    avoid backlight washout.*/
};
/**
 * Gain adjustment applied to the sensor or on the ipipe.*/
enum rraew_gain_type {
    GAIN_SENSOR = 0, /**<Perform gain adjustment before obtaining
    the statistics.*/
    GAIN_DIGITAL /**<Perform gain adjustment after obtaining
    the statistics*/
};

/**
 * Bayer pattern order. 
 * In a Bayer pattern there are three different color filters. 
 * They have the colors red(R), green(G), and blue(B) on a 2x2 grid of 
 * pixels. Two are always green and these always lie on a diagonal. 
 * This results in four possible constellations for the three colors.\n
 * Here you must specify the order in which the R, Gr, Gb and B 
 * colors are sent to the statistical hardware module. 
 * The values are from 0 to 3, where 0 represents the first pixel position.\n
 * \note To avoid filling this struct by your self, you can use:
 *  #colorptn_GrRBGb, #colorptn_BGbGrR, #colorptn_RGrGbB or
 *  #colorptn_GbBRGr
 */
struct rraew_colorpattern {
    /**Position of the red channel*/
    unsigned char r_offset;
    /**Position of the green-red channel*/
    unsigned char gr_offset;
    /**Position of the green-blue channel*/
    unsigned char gb_offset;
    /**Position of the blue channel*/
    unsigned char b_offset;
};

/**
 *  \par Pattern: 
 *  	Gr R\n
 *  	B  Gb 
 */
extern const struct rraew_colorpattern colorptn_GrRBGb;
/**
 *  \par Pattern:
 *  	B  Gb\n
 *  	Gr R
 */
extern const struct rraew_colorpattern colorptn_BGbGrR;
/**
 *  \par Pattern:
 *  	R  Gr\n
 *  	Gb B
 */
extern const struct rraew_colorpattern colorptn_RGrGbB;
/**
 *  \par Pattern:
 *  	Gb B\n
 *  	R  Gr
 */
extern const struct rraew_colorpattern colorptn_GbBRGr;

/**
 * Range step information.
 * This structure represents the gain step for a given range respect
 * to the previous range (to be used in an sorted array).
 */
struct rraew_gain_step {
    /**End of the gain range*/
    float range_end;
    /**Numerator of the gain step*/
    int step_n;
    /**Denominator of the gain step*/
    int step_d;
};

/** 
 * Statistics hardware module configuration including user-defined
 * windowing.
 */
struct rraew_stat_config {
    /** Number of windows in the vertical direction.*/
    short vt_cnt;
    /** Number of windows in the horizontal direction.*/
    short hz_cnt;
    /** Width of each window.*/
    short win_width;
    /** Height of each window.*/
    short win_height;
    /** Pointer to extra data needed for the functions that use statistics.*/
    void *private;
};

struct rraew;
/**
 * Hardware file descriptors for the capture device, 
 * the previewer device and the aew device.
 */
struct rraew_file_descriptors {
    /** Previewer file descriptor. */
    int previewer_fd;
    /** AEW engine file descriptor. */
    int aew_fd;
    /** Sensor file descriptor. */
    int capture_fd;
    /** Boolean tracking if the previewer file descriptor was created 
     * by librraew (non-zero) or was passed in by the user (zero).*/
    char owner_previewer_fd;
    /** Boolean tracking if the aew file descriptor was created 
     * by librraew (non-zero) or was passed in by the user (zero).*/
    char owner_aew_fd;
    /** Boolean tracking if the capture file descriptor was created 
     * by librraew (non-zero) or was passed in by the user (zero).*/
    char owner_capture_fd;
};

/**
 * Per window  statistical data: : average, maximum and minimum 
 * of each channel component
 */
struct rraew_stat {
    /**Red average*/
    unsigned int r_avg;
    /**Green average*/
    unsigned int g_avg;
    /**Blue average*/
    unsigned int b_avg;
    /**Red maximum*/
    unsigned int r_max;
    /**Green maximum*/
    unsigned int g_max;
    /**Blue maximum*/
    unsigned int b_max;
    /**Red minimum*/
    unsigned int r_min;
    /**Green minimum*/
    unsigned int g_min;
    /**Blue minimum*/
    unsigned int b_min;
};
/**
 * Function prototype to set RGB gains.
 * @param fd file descriptor of the device with the gains module
 * @param owner_fd flag that tell who is the file descriptor owner
 * @param q10r_gain gain value for the red component. The value format is fixed-point Q22:10
 * @param q10g_gain gain value for the green component. The value format is fixed-point Q22:10
 * @param q10b_gain gain value for the blue component. The value format is fixed-point Q22:10
 * @return 0 if the operation was successful or -1 otherwise
 */
typedef int (*RraewSetGain) (int *fd, char *owner_fd, __u32 q10r_gain,
    __u32 q10g_gain, __u32 q10b_gain, void *data);
/**
 * Function prototype to get RGB gains.
 * @param fd file descriptor of the device with the gains module
 * @param owner_fd flag that tell who is the file descriptor owner
 * @param q10r_gain pointer to get the red gain component on fixed-point Q22:10 format
 * @param q10g_gain pointer to get the green gain on fixed-point Q22:10 format
 * @param q10b_gain pointer to get the blue component on fixed-point Q22:10 format
 * @return 0 if the operation was successful or -1 otherwise
 */
typedef int (*RraewGetGain) (int *fd, char *owner_fd, __u32 * q10r_gain,
    __u32 * q10g_gain, __u32 * q10b_gain, void *data);
/** 
 * Function prototype to set exposure time 
 * @param fd capture device file descriptor 
 * @param owner_fd flag that tell who is the file descriptor owner
 * @param exp_time exposure time in us
 */
typedef int (*RraewSetExposure) (int *capture_fd, char *owner_capture_fd,
    __u32 exp_time, void *data);
/** 
 * Function prototype to get exposure time 
 * @param fd capture device file descriptor 
 * @param owner_fd flag that tell who is the file descriptor owner
 * @param exp_time pointer to get the exposure time in us
 */
typedef int (*RraewGetExposure) (int *capture_fd, char *owner_capture_fd,
    __u32 * exp_time, void *data);
/**
 * Sensor characteristics. Struct to gather the set of camera's features
 * that have to provide. Including ranges of the gains and 
 * exposure time, pointers to functions for set/get gains and
 * exposure time.
 */
struct rraew_sensor {
    /** Bayer pattern order.
     * There are different configuration of the RGB pixels in a Bayer 
     * pattern. It is important to set the correct one (given by the sensor) 
     * for satisfactory algorithm results*/
    struct rraew_colorpattern colorptn;
    /** Maximum exposure time.
     * Defines the maximun value that can be asigned to the exposure time.\n
     * Should be in the same units that the exposure time in the 
     * set/get exposure function*/
    int max_exp_time;
    /** Minimum exposure time.
	 * Defines the minimun value that can be asigned to the exposure time.\n
     * Should be in the same units that the exposure time in the 
     * set/get exposure function*/
    int min_exp_time;
    /** Maximum gain.
     * Defines the maximun value that can be asigned to the sensor gain, 
     * so it refers to the maximun value allowed by the set gain function\n
     * Use floating point NOT fixed point*/
    float max_gain;
    /** Minimum gain.
     * Defines the minimun value that can be asigned to the sensor gain, 
     * so it refers to the minimun value allowed by the set gain function.
     * Use floating point NOT fixed point*/
    float min_gain;
    /**Number of gain ranges.
     * Usually the gain in a sensor can be divided in different ranges
     * with different increment steps. For example the mt9p031 supports 
     * 3 ranges:
     * - 1-4 increments in steps of 0.125
     * - 4.25-8 increments in steps of 0.25
     * - 9-128 increments in steps of 1\n
     * Here you define the number of available ranges, the steps for each 
     * range is defined with #gain_steps*/
    int n_gain_steps;
    /** Gain steps for the different ranges. 
     * The array must be incrementally ordered, that means the lower
     * range must be the firts element in the array.*/
    struct rraew_gain_step *gain_steps;
    /** Pointer to function that sets the sensor gain (before the statistics hardware) */
    RraewSetGain set_gain;
    /** Pointer to the instance that contains the set_gain function. 
     * For C applications this pointer can be NULL*/
    void *set_gain_target;
    /** Pointer to function that gets the sensor gain (before the statistics hardware) */
    RraewGetGain get_gain;
    /** Pointer to the instance that contains the get_gain function. 
     * For C applications this pointer can be NULL*/
    void *get_gain_target;
    /** Pointer to function that sets exposure time on the sensor */
    RraewSetExposure set_exposure;
    /** Pointer to the instance that contains the set_exposure function. 
     * For C applications this pointer can be NULL*/
    void *set_exposure_target;
    /** Pointer to function that gets exposure time on the sensor */
    RraewGetExposure get_exposure;
    /** Pointer to the instance that contains the get_exposure function. 
     * For C applications this pointer can be NULL*/
    void *get_exposure_target;
};

/**
 * Function prototype to set parameters to the statistical module.
 * @param aew_fd, file descriptor of the aew device
 * @param owner_aew_fd flag that tell who is the file descriptor owner
 * @param width  number of horizontal pixels in the image
 * @param height  number of vertical pixels in the image
 * @param stat_config  pointer to struct that has the configuration 
 *        required for the statistical module
 * @return 0 if the operation was successful or -1 otherwise
 */
typedef int (*RraewSetStatParameters) (int *aew_fd, char *owner_aew_fd,
    int width, int height, struct rraew_stat_config * stat_config, void *data);
/**
 * Function prototype to read statistical data.
 * @param aew_fd, file descriptor of the aew device
 * @param owner_aew_fd flag that tell who is the file descriptor owner
 * @param stat_config  pointer to struct that has the configuration 
 * required for the statistical module
 * @param stats  struct array where this function leaves the statistical 
 * data. The array must start in order with the top/left window and end 
 * with the bottom/right window
 * @param colorptn  struct that contains the color pattern given by the sensor
 * @return 0 if the operation was successful or -1 otherwise
 */
typedef int (*RraewReadStatData) (int *aew_fd, char *owner_aew_fd,
    struct rraew_stat_config * stat_config, struct rraew_colorpattern colorptn,
    struct rraew_stat * stats, void *data);
/**
 * Function prototype to release statistical data.
 * @param aew_fd, file descriptor of the aew device
 * @param owner_aew_fd flag that tell who is the file descriptor owner
 * @param stat_config  pointer to struct that has the configuration 
 * required for the statistical module
 * @param stats  pointer to memory reserved for the statistical data
 * @return 0 if the operation was successful or -1 otherwise
 */
typedef int (*RraewReleaseStatData) (int *aew_fd, char *owner_aew_fd,
    struct rraew_stat_config * stat_config, struct rraew_stat * stats,
    void *data);
/**
 * Struct that contains all the required information of the interface.
 * 
 * Includes functions to access ipipe gains and statistical data and 
 * information about statistic configuration parameters and gain ranges
 * \note If you are using the dm365 interface, avoid filling this struct 
 * using #dm365_vpfe_interface.
 */
struct rraew_interface {
    /** Stats parameters */
    /** Maximum number of vertical windows*/
    int stat_max_v_window;
    /** Maximum number of pixels in vertical direction per window */
    int stat_max_pxl_v_window;
    /** Minimum number of pixels in vertical direction per window */
    int stat_min_pxl_v_window;
    /** Maximum number of horizontal windows*/
    int stat_max_h_window;
    /** Maximum number of pixels in horizontal direction per window */
    int stat_max_pxl_h_window;
    /** Minimum number of pixels in horizontal direction per window */
    int stat_min_pxl_h_window;
    /** Maximum gain.
     * Defines the maximun value that can be asigned to the sensor gain, 
     * so it refers to the maximun value allowed by the set gain function.\n
     * Use floating point NOT fixed point*/
    float max_gain;
    /** Minimum gain. 
     * Defines the minimun value that can be asigned to the sensor gain, 
     * so it refers to the minimun value allowed by the set gain function.\n
     * Use floating point NOT fixed point*/
    float min_gain;
    /**Number of gain ranges*/
    int n_gain_steps;
    /**
     * The structure contains the gain steps
     * for the different ranges. The array should be 
     * incrementally ordered. 
     */
    const struct rraew_gain_step *gain_steps;
    /** Sets the digital gain of each channel RGB*/
    RraewSetGain set_gain;
    /** Pointer to the instance that contains the set_gain function. 
     * For C applications this pointer can be NULL*/
    void *set_gain_target;
    /** Gets the digital gain of each channel RGB*/
    RraewGetGain get_gain;
    /** Pointer to the instance that contains the get_gain function. 
     * For C applications this pointer can be NULL*/
    void *get_gain_target;
    /**
     * Set parameters for the aew statistical module. This function has to 
     * configure the statistical module taking into account the information 
     * given by the rraew_stat_config 
     */
    RraewSetStatParameters set_stat_parameters;
    /** Pointer to the instance that contains the set_stat_parameters function. 
     * For C applications this pointer can be NULL*/
    void *set_stat_parameters_target;
    /**
     * Read data from the statistical module and parse it into a 
     * 'rraew_stat' struct array that organized the data of each 
     * windows sequentially.
     */
    RraewReadStatData read_stat_data;
    /** Pointer to the instance that contains the read_stat_data function. 
     * For C applications this pointer can be NULL*/
    void *read_stat_data_target;
    /** Release memory used for the statistical data */
    RraewReleaseStatData release_stat_data;
    /** Pointer to the instance that contains the release_stat_data function. 
     * For C applications this pointer can be NULL*/
    void *release_stat_data_target;
    void *private;
};

/**
 * Center point of the rectangle of interest.
 */
struct rraew_coordinates {
    /** x coordinate of the center point.
     *  The value can range from 0 to the image's width - 1*/
    unsigned int x;
    /** y coordinate of the center point.
     *  The value can range from 0 to the image's height - 1*/
    unsigned int y;
    /** Flag that indicates if the rectangle must be centered on the image.
     * if centered is enabled (=1) the variables x and y  are ignored */
    unsigned char centered;
};

struct rectangle_area {
    int width;
    int height;
    int col_start;
    int row_start;
    int idx;
    int size;
};

/** 
 * Auto white balance configuration settings.
 * This includes all the configuration options for auto white balance
 */
struct rraew_awb_configuration {
    /** Algorithm of auto white balance to use*/
    enum rraew_white_balance_algo algorithm;
    /** Gain type to use. Whatever the gains are going to 
     * be applied on the sensor or the video processing 
     * subsystem of the SoC*/
    enum rraew_gain_type gain_type;
};

struct rraew_white_balance {
    int (*init) (struct rraew * aew);
    int (*start) (struct rraew * aew);
    int (*close) (struct rraew * aew);
    struct rraew_awb_configuration config;
    /* Algorithm private data */
    void *private;
};

/** 
 * Auto exposure configuration settings.
 * This includes all the configuration options for auto exposure.
 */
struct rraew_ae_configuration {
    /** Auto exposure algorithm to use*/
    enum rraew_exposure_algo algorithm;
    /** Metering system to use.
     * Refers to how the image's luminance should be calculated */
    enum rraew_metering_type meter_type;
    /** Center point of the rectangle of interest.
     * Defines the coordinates where the rectangle of interest's center
     * is going to be placed.*/
    struct rraew_coordinates rect_center_point;
    /**Percentange of the rectangle of interest.
     * Some metering system use a frame's region to get 
     * its calculations attention. 
     * This parameter(rp) defines the size of that area 
     * as a percentage of the image width and height. So the area is a rectangle of 
     *rp(image_width) x rp(image_height).*/
    int rect_percentage;
};

struct rraew_exposure {
    int min_gain;
    unsigned char autogain;
    int (*init) (struct rraew * algo);
    int (*start) (struct rraew * algo);
    int (*close) (struct rraew * algo);
    __u32 (*metering) (struct rraew * algo);
    struct rraew_ae_configuration config;
    void *private;
};

/** 
 * General aew library configuration settings
 */
struct rraew_configuration {
    /**Width of the captured image*/
    int width;
    /**Height of the captured image */
    int height;
    /**Segmentation factor. 
     * Each frame is segmented into regions, this factor 
     * represents the percentage of the maximum number of possible regions. As 
     * bigger the percentage most cpu is used. \n
     * The segmentation factor can range from 10% to 100%*/
    int segmentation_factor;
};

struct rraew {
    struct rraew_white_balance wb;
    struct rraew_exposure ae;
    struct rraew_sensor sensor;
    struct rraew_interface interface;
    struct rraew_stat_config stat_config;
    struct rraew_stat *stats;
    struct rraew_configuration config;
    struct rraew_file_descriptors file_descriptors;
};

/**
 * Skeleton of values for DM365 vpfe interface. For the DM365 platform the 
 * librraew has a filled rraew_interface structure. The librraew implements
 * the required function callbacks. 
 */
extern const struct rraew_interface dm365_vpfe_interface;
/**
 * Creates an Auto White Balance/ Auto Exposure Algorithm handler
 * This function makes the structure of the aew library(instance), prepares the 
 * hardware modules and initialize the auto-exposure and auto-white-balance 
 * algorithms. All the structures are copied so the pointer can be 
 * released after the creation of the algorithm.
 * @param awb_config auto white balance configuration settings.
 * @param ae_config auto exposure configuration settings.
 * @param aew_config auto white balance and auto exposure common configuration 
 *  settings.
 * @param sensor structure that defines the sensor. 
 * @param interface structure that defines the video processing 
 *   subsystem of this SoC. 
 * @param fd each device can be accessed through file descriptors, obtained with 
 * the “open” function. librraew interacts with three devices: previewer 
 * (from video processing subsystem) , camera sensor and aew statistics engine. 
 * This parameter defines the file descriptors for these devices and the owner 
 * of each one. The ownership determines if the device has to be open by librraew 
 * or you give the file descriptor. When  the owner  is librraew, you may leave 
 * the file descriptor  NULL. 
 * @return an aew algorithm handler pointer or NULL on error.
 */
struct rraew *rraew_create(struct rraew_awb_configuration *awb_config,
    struct rraew_ae_configuration *ae_config,
    struct rraew_configuration *aew_config, struct rraew_sensor *sensor,
    struct rraew_interface *interface, struct rraew_file_descriptors *fd);
/**
 * Destroys an Auto White Balance/ Auto Exposure Algorithm. 
 * Free the memory used by the algorithm
 * @param aew an aew algorithm handler
 */
void rraew_destroy(struct rraew *aew);
/**
 * Executes one iteration of the auto exposure and auto white balance algorithm
 * @param aew an aew algorithm handler 
 */
int rraew_run(struct rraew *aew);

/**
 * Gets the coordinates for the rectangle of interest that is being used
 * for the auto exposure algorithm. Specifies the coordinates of two points: 
 * the top left and bottom right corners of the rectangle. The sides of the 
 * rectangle extend from these two points and are parallel to the x-axis and 
 * y-axis.
 * @param aew an aew algorithm handler
 * @param *right the coordinate value of the rectangle's right side.
 * @param *left the coordinate value of the rectangle's left side.
 * @param *top the coordinate value of the rectangle's top side.
 * @param *bottom the coordinate value of the rectangle's bottom side.
 * @return if the algorithm doesn't use a rectangle of interest, returns -1, 
 * otherwise returns 0 and the coordinates values are filled. 
 */
int rraew_get_rectangle_coordinates(struct rraew *aew, unsigned int *right,
    unsigned int *left, unsigned int *top, unsigned int *bottom);
#endif // RR_AEW
