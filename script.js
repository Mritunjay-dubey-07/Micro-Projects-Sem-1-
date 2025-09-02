
// Wait for DOM to be fully loaded
document.addEventListener('DOMContentLoaded', function() {
    
    // Get all interactive elements
    const loginForm = document.getElementById('login-form');
    const usernameInput = document.getElementById('username');
    const passwordInput = document.getElementById('password');
    const primaryBtns = document.querySelectorAll('.primary-btn');
    const secondaryBtns = document.querySelectorAll('.secondary-btn');
    const textBtns = document.querySelectorAll('.text-btn');
    
    // Add ripple effect to buttons
    function createRipple(event) {
        const button = event.currentTarget;
        const circle = document.createElement('span');
        const diameter = Math.max(button.clientWidth, button.clientHeight);
        const radius = diameter / 2;
        
        circle.style.width = circle.style.height = `${diameter}px`;
        circle.style.left = `${event.clientX - button.offsetLeft - radius}px`;
        circle.style.top = `${event.clientY - button.offsetTop - radius}px`;
        circle.classList.add('ripple');
        
        // Add ripple styles
        circle.style.position = 'absolute';
        circle.style.borderRadius = '50%';
        circle.style.background = 'rgba(255, 255, 255, 0.4)';
        circle.style.transform = 'scale(0)';
        circle.style.animation = 'ripple 0.6s linear';
        circle.style.pointerEvents = 'none';
        
        // Remove existing ripples
        const ripple = button.getElementsByClassName('ripple')[0];
        if (ripple) {
            ripple.remove();
        }
        
        button.appendChild(circle);
    }
    
    // Add CSS animation for ripple effect
    const style = document.createElement('style');
    style.textContent = `
        @keyframes ripple {
            to {
                transform: scale(4);
                opacity: 0;
            }
        }
        
        .input-focus-glow {
            box-shadow: 0 0 20px rgba(50, 130, 184, 0.3);
        }
        
        .btn-click-animation {
            animation: btnClick 0.1s ease;
        }
        
        @keyframes btnClick {
            0% { transform: scale(1); }
            50% { transform: scale(0.95); }
            100% { transform: scale(1); }
        }
        
        .shake {
            animation: shake 0.5s ease-in-out;
        }
        
        @keyframes shake {
            0%, 100% { transform: translateX(0); }
            25% { transform: translateX(-5px); }
            75% { transform: translateX(5px); }
        }
    `;
    document.head.appendChild(style);
    
    // Add ripple effect to all buttons
    [...primaryBtns, ...secondaryBtns].forEach(button => {
        button.addEventListener('click', createRipple);
        
        // Add click animation
        button.addEventListener('mousedown', function() {
            this.classList.add('btn-click-animation');
        });
        
        button.addEventListener('animationend', function() {
            this.classList.remove('btn-click-animation');
        });
    });
    
    // Enhanced input focus effects
    [usernameInput, passwordInput].forEach(input => {
        input.addEventListener('focus', function() {
            this.parentElement.style.transform = 'scale(1.02)';
            this.parentElement.classList.add('input-focus-glow');
        });
        
        input.addEventListener('blur', function() {
            this.parentElement.style.transform = 'scale(1)';
            this.parentElement.classList.remove('input-focus-glow');
        });
        
        // Add typing animation
        input.addEventListener('input', function() {
            this.style.transform = 'scale(1.01)';
            setTimeout(() => {
                this.style.transform = 'scale(1)';
            }, 100);
        });
    });
    
    // Form submission handling
    loginForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        const username = usernameInput.value.trim();
        const password = passwordInput.value.trim();
        
        if (!username || !password) {
            // Shake animation for empty fields
            if (!username) {
                usernameInput.parentElement.classList.add('shake');
                usernameInput.focus();
            }
            if (!password && username) {
                passwordInput.parentElement.classList.add('shake');
                passwordInput.focus();
            }
            
            // Remove shake animation after completion
            setTimeout(() => {
                usernameInput.parentElement.classList.remove('shake');
                passwordInput.parentElement.classList.remove('shake');
            }, 500);
            
            return;
        }
        
        // Simulate login process
        const loginBtn = document.querySelector('.login-btn');
        const originalText = loginBtn.querySelector('.btn-text').textContent;
        
        loginBtn.querySelector('.btn-text').textContent = 'Signing In...';
        loginBtn.style.background = 'linear-gradient(135deg, #28a745, #20c997)';
        loginBtn.disabled = true;
        
        // Simulate API call delay
        setTimeout(() => {
            loginBtn.querySelector('.btn-text').textContent = '✓ Success!';
            loginBtn.style.background = 'linear-gradient(135deg, #28a745, #20c997)';
            
            setTimeout(() => {
                alert(`Welcome back, ${username}! Login successful.`);
                loginBtn.querySelector('.btn-text').textContent = originalText;
                loginBtn.style.background = 'linear-gradient(135deg, var(--highlight-color), #5ba7db)';
                loginBtn.disabled = false;
                
                // Reset form
                loginForm.reset();
            }, 1000);
        }, 1500);
    });
    
    // Handle other buttons
    document.querySelector('.signup-btn').addEventListener('click', function() {
        alert('Sign-up functionality coming soon!');
    });
    
    document.querySelector('.forgot-btn').addEventListener('click', function() {
        alert('Password reset link will be sent to your email.');
    });
    
    // Add smooth scrolling and parallax effect
    window.addEventListener('scroll', function() {
        const scrolled = window.pageYOffset;
        const parallax = document.querySelector('.background-blur');
        const speed = scrolled * 0.5;
        
        parallax.style.transform = `translateY(${speed}px)`;
    });
    
    // Add hover sound effect simulation (visual feedback)
    function addHoverEffect(element) {
        element.addEventListener('mouseenter', function() {
            this.style.filter = 'brightness(1.1)';
        });
        
        element.addEventListener('mouseleave', function() {
            this.style.filter = 'brightness(1)';
        });
    }
    
    // Apply hover effects to interactive elements
    [...primaryBtns, ...secondaryBtns, ...textBtns].forEach(addHoverEffect);
    
    // Add floating animation to logo
    const logo = document.querySelector('.logo-text');
    setInterval(() => {
        logo.style.transform = 'scale(1.02)';
        setTimeout(() => {
            logo.style.transform = 'scale(1)';
        }, 200);
    }, 3000);
    
    // Dynamic background color shift
    let hue = 210;
    setInterval(() => {
        hue = (hue + 0.5) % 360;
        document.documentElement.style.setProperty('--accent-color', `hsl(${hue}, 60%, 25%)`);
    }, 100);
    
    console.log('🏦 Bank of Diddy Oil - Advanced UI Loaded Successfully!');
});
