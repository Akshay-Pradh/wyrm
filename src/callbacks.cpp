
void NATNET_CALLCONV dataHandler(sFrameOfMocapData* data, void* pUserData)
{
    NatNetClient* pClient = (NatNetClient*) pUserData;
    if (!pClient)
        return;

    // Note : This function is called every 1 / mocap rate ( e.g. 100 fps = every 10 msecs )
    // We don't want to do too much here and cause the network processing thread to get behind,
    // so let's just safely add this frame to our shared  'network' frame queue and return.
    
    // Note : The 'data' ptr passed in is managed by NatNet and cannot be used outside this function.
    // Since we are keeping the data, we need to make a copy of it.
    shared_ptr<sFrameOfMocapData> pDataCopy = make_shared<sFrameOfMocapData>();
    NatNet_CopyFrame(data, pDataCopy.get());

    MocapFrameWrapper f;
    f.data = pDataCopy;
    f.clientLatencyMillisec = pClient->SecondsSinceHostTimestamp(data->CameraMidExposureTimestamp) * 1000.0;
    f.transitLatencyMillisec = pClient->SecondsSinceHostTimestamp(data->TransmitTimestamp) * 1000.0;

    if (gNetworkQueueMutex.try_lock_for(std::chrono::milliseconds(5)))
    {
        gNetworkQueue.push_back(f);

        // Maintain a cap on the queue size, removing oldest as necessary
        while ((int)gNetworkQueue.size() > kMaxQueueSize)
        {
            f = gNetworkQueue.front();
            NatNet_FreeFrame(f.data.get());
            gNetworkQueue.pop_front();
        }
        gNetworkQueueMutex.unlock();
    }
    else
    {
        // Unable to lock the frame queue and we chose not to wait - drop the frame and notify
        NatNet_FreeFrame(pDataCopy.get());
        printf("\nFrame dropped (Frame : %d)\n", f.data->iFrame);
    }

    return;
}