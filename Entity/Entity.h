#ifndef ___Entity_h
#define ___Entity_h

class Entity {
public:
    Entity();
    virtual ~Entity();

    virtual void Update();
    virtual void Render();

    bool IsAlive() { return alive; }
    void Die();

private:
    bool alive;
};

#endif // ___Entity_h


