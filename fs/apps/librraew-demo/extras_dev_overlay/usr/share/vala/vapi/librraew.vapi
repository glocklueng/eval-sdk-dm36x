/*
 * Copyright RidgeRun (C) 2010
 */

[CCode(lower_case_cprefix = "", cheader_filename = "rraew.h")]
 namespace rraew {

     [CCode(cprefix = "AWB_", cname = "enum rraew_white_balance_algo")]
     public enum RraewWhiteBalanceAlgo {
         NONE = 0,
         GRAY_WORLD, 
         WHITE_PATCH,
         WHITE_PATCH_2
     }

     [CCode(cprefix = "AE_", cname = "enum rraew_exposure_algo")]
     public enum RraewExposureAlgo {
         NONE = 0,
         EC
     }

     [CCode(cprefix = "METER_", cname = "enum rraew_metering_type")]
     public enum RraewMeteringType {
         PARTIAL_AREA = 0,
         RECT_WEIGHTED,
         AVERAGE,
         SEGMENT
     }

     [CCode(cprefix = "GAIN_", cname = "enum rraew_gain_type")]
     public enum RraewGainType {
         SENSOR = 0,
         DIGITAL
     }

     [CCode(cname = "struct rraew_colorpattern")]
     public struct RraewColorPattern {
         uchar r_offset;
         uchar gr_offset;
         uchar gb_offset;
         uchar b_offset;
     }
     
     [CCode(cname = "struct rraew_coordinates")]
     public struct RraewCoordinates {
        uint x;
        uint y;
        uchar centered;
     }

     public const RraewColorPattern colorptn_GrRBGb;
     public const RraewColorPattern colorptn_BGbGrR;
     public const RraewColorPattern colorptn_RGrGbB;
     public const RraewColorPattern colorptn_GbBRGr;

     [CCode(cname = "struct rraew_gain_step")]
     public struct RraewGainStep {
         float range_end;
         int step_n;
         int step_d;
     }

     [CCode(cname = "struct rraew_stat_config")]
     public struct RraewStatConfig {
         short vt_cnt;
         short hz_cnt;
        short win_width;
        short win_height;
        void *private;
     }
     
     [CCode(cname = "struct rraew_stat")]
     public struct RraewStat {
         int r_avg;
         int g_avg;
         int b_avg;
         int r_max;
         int g_max;
         int b_max;
         int r_min;
         int g_min;
         int b_min;
     }

     [CCode(cname = "struct rraew_file_descriptors")]
     public struct RraewFileDescriptors {
         int previewer_fd;
         int aew_fd;
         int capture_fd;
         char owner_previewer_fd;
         char owner_aew_fd;
         char owner_capture_fd;
     }
     
     [CCode(cname = "struct rraew_awb_configuration")]
     public struct RraewAwbConfiguration {
         RraewWhiteBalanceAlgo algorithm;
         RraewGainType gain_type;
     }
     
     [CCode(cname = "struct rraew_ae_configuration")]
     public struct RraewAeConfiguration {
         RraewExposureAlgo algorithm;
         RraewMeteringType meter_type;
         RraewCoordinates rect_center_point;
         int rect_percentage;
     }
    
     [CCode(cname = "struct rraew_configuration")]
     public struct RraewConfiguration {
         int width;
         int height;
         int segmentation_factor;
     }
    
     /* Sensor delegates */
    [CCode (cname = "RraewSetGain")]
    public delegate int RraewSetGain(int *fd, char *owner_fd,
         uint32 r_gain, uint32 g_gain, uint32 b_gain);
     [CCode (cname = "RraewGetGain")]
     public delegate int RraewGetGain(int *fd, char *owner_fd,
         uint32 *r_gain, uint32 *g_gain, uint32 *b_gain);
     [CCode (cname = "RraewSetExposure")]
     public delegate int RraewSetExposure(int *capture_fd, char *owner_capture_fd,
         uint32 exp_time);
     [CCode (cname = "RraewGetExposure")]
     public delegate int RraewGetExposure(int *capture_fd, char *owner_capture_fd,
         uint32 *exp_time);

     [CCode(cname = "struct rraew_sensor")]
     public struct RraewSensor {
         public RraewColorPattern colorptn;
         public int max_exp_time;
         public int min_exp_time;
         public float max_gain;
         public float min_gain;
         public int n_gain_steps;
         public RraewGainStep *gain_steps;
         public unowned RraewSetGain set_gain;
         public unowned RraewGetGain get_gain;
         public unowned RraewSetExposure set_exposure;
         public unowned RraewGetExposure get_exposure;
     }
     /* Interface delegates */
     [CCode (cname = "RraewSetStatParameters")]
     public delegate int RraewSetStatParameters(int *aew_fd, char *owner_aew_fd,
         int width, int height, RraewStatConfig * stat_config);
     [CCode (cname = "RraewReadStatData")]
     public delegate ssize_t RraewReadStatData(int *aew_fd, char *owner_aew_fd,
         RraewStatConfig * stat_config, RraewStat * stats, RraewColorPattern colorptn);
     [CCode (cname = "RraewReleaseStatData")]
     public delegate int RraewReleaseStatData(int *aew_fd, char *owner_aew_fd,
         RraewStatConfig * stat_config, RraewStat * stats);
     [CCode(cname = "struct rraew_interface")]
     public struct RraewInterface {
         int stat_max_v_window;
         int stat_max_pxl_v_window;
         int stat_max_h_window;
         int stat_max_pxl_h_window;
         float max_gain;
         float min_gain;
         int n_gain_steps;
         public RraewGainStep *gain_steps;
         public unowned RraewSetGain set_gain;
         public unowned RraewGetGain get_gain;
         public unowned RraewSetStatParameters set_stat_parameters;
         public unowned RraewReadStatData read_stat_data;
         public unowned RraewReleaseStatData release_stat_data;
     }
     public const RraewInterface dm365_vpfe_interface;

     [CCode(cname = "rraew_create")]
     void *rraew_create(RraewAwbConfiguration *awb_config,
        RraewAeConfiguration *ae_config, RraewConfiguration *aew_config,
        RraewSensor *sensor, RraewInterface *interface, RraewFileDescriptors *fd);

     [CCode(cname = "rraew_destroy")]
     void rraew_destroy(void *aew);

     [CCode(cname = "rraew_run")]
     int rraew_run(void *aew);
     
     [CCode(cname = "rraew_get_rectangle_coordinates")]   
     int rraew_get_rectangle_coordinates(void *aew, uint *right, uint *left, 
        uint *top, uint *bottom);

 }
