# Space Transform Summary

## 1. Object Space -> 2. World Space -> 3. Camera Space -> 4. Clip Space -> 5. Screen Space

### 1. Object Space -> 2. World Space (World Transform)
- vertex의 위치는 필요에 따라 Scale, Rotate, Transition을 하면 된다. [L|t]
- normal vector은 바꾸기 위해서는 L^(-T) 행렬을 곱한다.

### 2. World Space -> 3. Camera Space (View Transform)
카메라 공간의 좌표계와 월드 공간의 좌표계를 맞춘다.
- 원점을 맞추고, 기저벡터끼리 맞도록 회전시킨다.
- M_view = RT ( = Transition -> Rotate)

### 3. Camera Space -> 4. Clip Space (Projection Transform)
- 카메라의 가시영역 (View frustum)
- 가시영역밖의 물체는 자른다. -> Clipping

![Projection Transform Matrix](https://i.stack.imgur.com/G4vP8.png)   


### 4. Clip Space -> 5. Screen Space (Viewport transform)
- Clip Space(NDC) to 실제 보이는 Screen Space
- Scale * Transition
- Scan conversion

![Viewport Transform Matrix](https://blog.kakaocdn.net/dn/UuhjK/btq18D9IB2Y/mZ7xMAPglOK8yj1JcaASZk/img.png)
