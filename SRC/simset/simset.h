#ifndef SIMSET_H
#define SIMSET_H

class Linkage {
friend class Link;
friend class Head;
public:
    Linkage();
    Link *Pred() const;
    Link *Suc() const;
    Linkage *Prev() const;
private:
    Linkage *PRED, *SUC;
    virtual Link *LINK() = 0;
};

class Head : public Linkage {
public:
    Head();
    Link *First() const;
    Link *Last() const;
    int Empty(void) const;
    int Cardinal(void) const;
    void Clear(void);
private:
    Link *LINK();
}; 

class Link : public Linkage {
public:
    void Out(void);
    void Into(Head *H);
    void Precede(Linkage *L);
    void Follow(Linkage *L);
private:
    Link *LINK();
};

#endif