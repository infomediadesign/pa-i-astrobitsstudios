#include "AttackSlam.h"
#include <cmath>

static Vector2 Add(Vector2 a, Vector2 b){ return {a.x+b.x,a.y+b.y}; }
static Vector2 Sub(Vector2 a, Vector2 b){ return {a.x-b.x,a.y-b.y}; }
static Vector2 Mul(Vector2 a, float s){ return {a.x*s,a.y*s}; }
static float Dot(Vector2 a, Vector2 b){ return a.x*b.x + a.y*b.y; }
static float Len(Vector2 a){ return sqrtf(a.x*a.x + a.y*a.y); }

static Vector2 Norm(Vector2 a)
{
    float l = Len(a);
    if(l<0.0001f) return {0,0};
    return {a.x/l,a.y/l};
}

AttackSlam::AttackSlam()
{
    Init();
}

void AttackSlam::Init()
{
    dirs.clear();

    dirs.push_back({0,-1});
    dirs.push_back({0,1});
    dirs.push_back({-1,0});
    dirs.push_back({1,0});

    Reset();
}

void AttackSlam::Reset()
{
    phase = PHASE_IDLE;
    timer = 0;
    active = false;
    hitApplied = false;
}

void AttackSlam::Start(Vector2)
{
    phase = PHASE_MARK;
    timer = 0;
    active = true;
    hitApplied = false;
}

bool AttackSlam::IsActive() const
{
    return active;
}

void AttackSlam::Update(float dt, Vector2, Vector2)
{
    if(!active) return;

    timer += dt;

    if(phase == PHASE_MARK)
    {
        if(timer >= markDuration)
        {
            phase = PHASE_SLAM;
            timer = 0;
        }
    }
    else if(phase == PHASE_SLAM)
    {
        if(timer >= slamDuration)
        {
            phase = PHASE_COOLDOWN;
            timer = 0;
        }
    }
    else if(phase == PHASE_COOLDOWN)
    {
        if(timer >= cooldownDuration)
        {
            Reset();
        }
    }
}

void AttackSlam::GetTriangle(Vector2 bossPos, Vector2 dir,
                             Vector2& p0, Vector2& p1, Vector2& p2) const
{
    Vector2 n = Norm(dir);
    Vector2 perp = {-n.y,n.x};

    p0 = bossPos;

    Vector2 tip = Add(bossPos, Mul(n,length));

    p1 = Add(tip, Mul(perp, halfWidth));
    p2 = Add(tip, Mul(perp,-halfWidth));
}

void AttackSlam::DrawTriangleAOE(Vector2 bossPos, Vector2 dir, Color c) const
{
    Vector2 p0,p1,p2;
    GetTriangle(bossPos,dir,p0,p1,p2);

    DrawTriangle(p0,p1,p2,c);
    DrawTriangleLines(p0,p1,p2,BLACK);
}

void AttackSlam::DrawSpikes(Vector2 bossPos, Vector2 dir, float t) const
{
    float wave = sinf(t*3.1415f);

    float h = spikeMaxHeight * wave;

    Vector2 n = Norm(dir);
    Vector2 perp = {-n.y,n.x};

    for(int i=1;i<=spikesPerArea;i++)
    {
        float u = (float)i/(spikesPerArea+1);

        Vector2 base = Add(bossPos, Mul(n,length*u));

        float width = halfWidth*u;

        for(int s=-1;s<=1;s+=2)
        {
            Vector2 b1 = Add(base, Mul(perp,width*0.2f*s));
            Vector2 b2 = Add(base, Mul(perp,(width*0.2f+7)*s));
            Vector2 apex = Add(base, Mul(n,h));

            DrawTriangle(b1,b2,apex,MAROON);
        }
    }
}

void AttackSlam::Draw(Vector2 bossPos) const
{
    if(!active) return;

    if(phase == PHASE_MARK)
    {
        for(auto d:dirs)
            DrawTriangleAOE(bossPos,d,Fade(RED,0.3f));
    }
    else if(phase == PHASE_SLAM)
    {
        float t = timer/slamDuration;

        for(auto d:dirs)
        {
            DrawTriangleAOE(bossPos,d,Fade(RED,0.6f));
            DrawSpikes(bossPos,d,t);
        }
    }
}

float AttackSlam::CheckDamage(float, Vector2 bossPos, Rectangle playerRect)
{
    if(!active) return 0;
    if(phase!=PHASE_SLAM) return 0;
    if(hitApplied) return 0;

    Vector2 center =
    {
        playerRect.x+playerRect.width/2,
        playerRect.y+playerRect.height/2
    };

    float radius = playerRect.width/2;

    for(auto d:dirs)
    {
        Vector2 p0,p1,p2;
        GetTriangle(bossPos,d,p0,p1,p2);

        if(CircleHitsTriangle(center,radius,p0,p1,p2))
        {
            hitApplied=true;
            return 1.0f;
        }
    }

    return 0;
}

bool AttackSlam::PointInTriangle(Vector2 p,Vector2 a,Vector2 b,Vector2 c) const
{
    Vector2 v0=Sub(c,a);
    Vector2 v1=Sub(b,a);
    Vector2 v2=Sub(p,a);

    float d00=Dot(v0,v0);
    float d01=Dot(v0,v1);
    float d02=Dot(v0,v2);
    float d11=Dot(v1,v1);
    float d12=Dot(v1,v2);

    float inv=1/(d00*d11-d01*d01);

    float u=(d11*d02-d01*d12)*inv;
    float v=(d00*d12-d01*d02)*inv;

    return u>=0 && v>=0 && u+v<=1;
}

float AttackSlam::DistPointSegment(Vector2 p,Vector2 a,Vector2 b) const
{
    Vector2 ab=Sub(b,a);

    float t=Dot(Sub(p,a),ab)/Dot(ab,ab);

    if(t<0)t=0;
    if(t>1)t=1;

    Vector2 proj=Add(a,Mul(ab,t));

    return Len(Sub(p,proj));
}

bool AttackSlam::CircleHitsTriangle(Vector2 c,float r,
                                    Vector2 a,Vector2 b,Vector2 d) const
{
    if(PointInTriangle(c,a,b,d)) return true;

    if(DistPointSegment(c,a,b)<=r) return true;
    if(DistPointSegment(c,b,d)<=r) return true;
    if(DistPointSegment(c,d,a)<=r) return true;

    return false;
}