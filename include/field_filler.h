#ifndef FUZZER_FIELD_FILLER_H
#define FUZZER_FIELD_FILLER_H

class field;

class field_filler {
public:
    virtual ~field_filler() = default;
    virtual void fill(field &) = 0;
};

#endif // FUZZER_FIELD_FILLER_H
