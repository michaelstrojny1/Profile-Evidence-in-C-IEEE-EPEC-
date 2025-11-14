**Profile Evidence in C++**

Profile Likelihood (PL) [2] assesses parameter $\theta$'s identifiability by plotting log likelihood vs $\theta$ while optimizing for the rest of the state $\nu$.
$\mathrm{PL}(\theta) = \max_{\nu} \log p(y|\theta,\nu)$
PL treats the optimizer's choice for $\nu$ as the true value, ignoring $\nu$'s uncertainty. We propose plotting the likelihood marginalized over $\nu$ $p(D\mid\theta)$; we call our method Profile Evidence (PE).

$p(D\mid\theta)=\int p(D\mid\theta,\nu)\,p(\nu\mid\theta)\,d\nu$

PE rewards $\theta$ that fit across a broad, plausible region of $\nu$, not just a (possibly wrong) $\nu$ optimum. Uncertainty quantification is crucial in power systems; we compare PE and PL for generator governor deadband estimates in a 5 generator network on a simplified IEEE14 grid [4].

<img width="866" height="472" alt="image" src="https://github.com/user-attachments/assets/890ff640-3b2c-4311-8f1c-26701626b699" />
