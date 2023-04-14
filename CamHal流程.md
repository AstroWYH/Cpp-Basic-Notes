### 1. Camera Provider 启动初始化

![image-20230414160618532](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20230414160618532.png)

hal进程provider进程启动

camera_module_t 调 HAL3Module 调 HwEnvironment

![image-20230414160725549](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20230414160725549.png)

camx chi-cdk互相调

### 2. 打开相机设备/初始化相机设备

**a) open**

App调CameraManager的openCamera

调ICameraDevice::open()这一个HIDL接口通知Provider

调HAL中的camera3_device_t结构体，获取一个camera设备

**b) initialize**

在调用open后紧接着被调用，主要用于将上层的回调接口传入HAL中

包括process_capture_result和notify的函数指针

### 3. 配置相机设备数据流

App调CameraDevice.createCaptureSession，并且通过Camera api v2标准接口，通知Camera Service

通过HIDL接口ICameraDeviceSession::configureStreams_3_4通知Provider开始处理此次配流需求

在Provider内部，调camera3_device_t的configure_streams将数据流配置传入CamX-CHI中，之后由CamX-CHI完成对数据流的配置工作

配流：

1. 根据**operation_mode**、camera 个数以及**stream的配置信息**选取了对应的**UsecaseId**
2. 根据所选取的**UsecaseId**，使用UsecaseFactory简单工厂类**创建**了用于管理整个场景下所有资源的**AdvancedCameraUsecase**对象
3. 创建AdvancedCameraUsecase对象是通过调用其**Create**()方法完成，该方法中获取了common_usecase.xml定义的关于Usecase的配置信息，之后又根据需要**创建了Feature**并选取了Feature所需的pipeline，并通过Override机制将Feature中所需要的Pipeline加入重建后的Usecase中
4. 最后通过调用CameraUsecaseBaese的initialize方法依次**创建了各个pipeline以及Session**，并且将AdvancedCameraUsecase的成员方法**注册到Session**，用于**Session将数据返回给Usecase中**。

### 4. 处理拍照请求

app调setRepeatingRequest。

通过Camera api v2接口下发到Camera Service中。

HIDL接口ICameraCaptureSession:processCaptureRequest_3_4将请求发送至Provider。

调用camera3_device_t结构体的process_capture_request到CamX-CHI。



CamX将request转发到**HALDevice**中，通过m_ChiAppCallbacks.**chi_override_process_request**将request**发送到CHI**

（chi_override_process_request方法的定义位于chxextensioninterface.cpp中）

在chi_override_process_request中会去获取**ExtensionModule**对象，并将request发送到ExtensionModule对象中，该对象中存储了之前创建的**Usecase对象**，然后经过层层调用，最终会调用**AdvancedCameraUsecase**的**ExecuteCaptureRequest**方法，该方法负责处理此次Request。

一路调到Session::StreamOn和**Session::ProcessCaptureRequest**，继续调到pipeline的函数：

1）完成Sensor以及IFE等Node的控制硬件模块出图前的配置，其中包括了曝光的参数的设置，Node的Buffer管理机制ImageBufferManager的创建，而该类用于管理Node中的output port的buffer申请/流转/释放等操作。

2）会进一步通知CSL部分开启数据流，分配用于装载图像数据的buffer。

![image-20230414164659016](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20230414164659016.png)

1. 通过调用**Session::ProcessCaptureRequest**方法进入到Session，然后调用**Pipeline::ProcessRequest**方法通知Pipeline开始处理此次Request。
2. 在Pipeline中，会先去调用内部的每一个Node的SetupRequest方法分别设置该Node的Output Port以及Input Port，之后通过调用**DRQ**(DeferredRequestQueue)的**AddDeferredNode**方法将**所有的Node**加入到DRQ中，其中DRQ中有两个队列分别是用于保存没有依赖项的Node的**m_readyNodes**以及保存处于等待依赖关系满足的Node的**m_deferredNodes**，当调用DRQ的**DispatchReadyNodes**方法后，会开始从**m_readyNodes**队列中**取出Node**调用其**ProcessRequest**开始进入Node内部处理本次request，在处理过程中会**更新meta data数据**，并更新至DRQ中，当该Node处理完成之后，会**将处于m_deferredNodes中的已无依赖关系的Node移到m_readyNodes**中，并再次调用DispatchReadyNodes方法从m_readyNodes取出Node进行处理。
3. 与此过程中，当**Node**的数据处理**完成**之后会通过**CSLFenceCallback通知到Pipeline**，此时Pipeline会判断当前Node的Output port **是否是Sink Port**(输出到CHI)，**如果不是**，则会更新依赖项到DRQ中，并且将不存在依赖项的Node移到m_readyNodes队列中，然后**调用DispatchReadyNdoes继续**进入到DRQ中流转，**如果是Sink Port**，则表示此Node是**整个Pipeline的最末端**，调用**sinkPortFenceSignaled**将数据给到Session中，最后通过调用Session中的**NotifyResult将结果发送到CHI**中。

![image-20230414165304238](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20230414165304238.png)

### 5. 上传拍照结果

在用户开启了相机应用，相机框架收到某次Request请求之后会开始对其进行处理，一旦有图像数据产生便会通过层层回调最终返回到应用层进行显示。



![image-20230414170546572](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20230414170546572.png)

![image-20230414170651339](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20230414170651339.png)

![image-20230414170752842](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20230414170752842.png)