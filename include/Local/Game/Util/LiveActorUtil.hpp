#pragma once

class LiveActor;

namespace MR {
    bool startBckIfExist(const LiveActor *, const char *);
    bool startBtkIfExist(const LiveActor *, const char *);
    bool startBrkIfExist(const LiveActor *, const char *);
    bool startBtpIfExist(const LiveActor *, const char *);
    bool startBpkIfExist(const LiveActor *, const char *);
    bool startBvaIfExist(const LiveActor *, const char *);
}