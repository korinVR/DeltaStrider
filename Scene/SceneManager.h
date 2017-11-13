#ifndef ___SceneManager_h
#define ___SceneManager_h

#include <boost/utility.hpp>
using namespace boost;

class Scene;

class SceneManager : noncopyable {
public:
    SceneManager(Scene* scene);
    virtual ~SceneManager();
    void Update();
    void Render();

    int GetFrame() const;
    
private:
    void ChangeScene(Scene* scene);
    
private:
    Scene* scene;
    Scene* nextScene;
    int frame;

    friend class Scene;
};

#endif // ___SceneManager_h


